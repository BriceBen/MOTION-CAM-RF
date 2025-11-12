#include <Arduino.h>
#include <M5Unified.h>
#include <WiFi.h>
#include <HTTPClient.h>

#include <FS.h>
#include <SD.h>
#include <SPI.h>

// ====== À PERSONNALISER ======
// Identifiants du hotspot et paramètres d’accès à la caméra.
// La Tab5 rejoindra le même réseau Wi-Fi que l’ESP32-CAM pour récupérer /capture.
const char* WIFI_SSID        = "Brice's Galaxy S10";
const char* WIFI_PASSWORD    = "ldv2019!";
const char* CAM_IP           = "10.126.43.118";
const uint16_t CAM_PORT      = 80;
const char* CAM_CAPTURE_PATH = "/capture";
// =============================

// Pour compatibilité Tab5
// Définition des broches SDIO si non fournies par l’environnement.
// Ces pins concernent l’ESP32 hôte; la Tab5 expose aussi la microSD en SPI ci-dessous.
#ifndef BOARD_SDIO_ESP_HOSTED_CLK
  #define SDIO2_CLK GPIO_NUM_12
  #define SDIO2_CMD GPIO_NUM_13
  #define SDIO2_D0  GPIO_NUM_11
  #define SDIO2_D1  GPIO_NUM_10
  #define SDIO2_D2  GPIO_NUM_9
  #define SDIO2_D3  GPIO_NUM_8
  #define SDIO2_RST GPIO_NUM_15
#endif

// Broches microSD du Tab5 (bus SPI)
// On utilise la microSD pour sauvegarder périodiquement des JPEG.
#define SD_SPI_CS_PIN   42
#define SD_SPI_SCK_PIN  43
#define SD_SPI_MOSI_PIN 44
#define SD_SPI_MISO_PIN 39

// Entrée déclenchement (G4)
// Le capteur PIR (via la M5 Tab5) met ce GPIO à HIGH lorsqu’un mouvement est détecté.
static const int PIN_TRIGGER = 4;  // G4 = GPIO4

// Cadences
// kMinIntervalMs : période minimale entre deux affichages (limitation framerate UI)
// kSaveIntervalMs : période mini entre deux enregistrements sur SD quand G4=HIGH
static const uint32_t kMinIntervalMs  = 80;    // rafraîchissement affichage
static const uint32_t kSaveIntervalMs = 3000;  // 3 secondes entre enregistrements quand G4=HAUT

// Variables d’horodatage (millis) pour cadencer fetch et sauvegarde
uint32_t lastFetchMs = 0;
uint32_t lastSaveMs  = 0;
bool     showingWaitUi = false;  // évite de redessiner en boucle l’écran d’attente

// ---------- SD helpers ----------
// Initialisation de la microSD en mode SPI avec un clock up to 25 MHz
bool beginSD_Tab5_SPI() {
  SPI.begin(SD_SPI_SCK_PIN, SD_SPI_MISO_PIN, SD_SPI_MOSI_PIN, SD_SPI_CS_PIN);
  if (!SD.begin(SD_SPI_CS_PIN, SPI, 25000000)) {
    Serial.println("[SD] SD.begin() a échoué (SPI)");
    return false;
  }
  Serial.println("[SD] SD SPI OK");
  return true;
}

// Vérifie/crée le répertoire /captures sur la carte SD
bool ensureCapturesDir() {
  if (!SD.begin(SD_SPI_CS_PIN, SPI, 25000000)) {
    if (!beginSD_Tab5_SPI()) return false;
  }
  if (!SD.exists("/captures")) {
    if (!SD.mkdir("/captures")) {
      Serial.println("[SD] mkdir /captures a échoué");
      return false;
    }
  }
  return true;
}

// Écrit un buffer JPEG en fichier, nom basé sur millis() pour unicité
bool saveJpegToSD(const std::vector<uint8_t>& jpg) {
  if (!ensureCapturesDir()) return false;

  char path[64];
  // Nom simple et unique basé sur millis()
  snprintf(path, sizeof(path), "/captures/cap_%010lu.jpg", (unsigned long)millis());

  File f = SD.open(path, FILE_WRITE);
  if (!f) {
    Serial.printf("[SD] open() échoué: %s\n", path);
    return false;
  }
  size_t w = f.write(jpg.data(), jpg.size());
  f.close();

  if (w != jpg.size()) {
    Serial.printf("[SD] write incomplet (%u/%u): %s\n",
                  (unsigned)w, (unsigned)jpg.size(), path);
    return false;
  }
  Serial.printf("[SD] écrit: %s (%u octets)\n", path, (unsigned)jpg.size());
  return true;
}

// ---------- Récupération JPEG ----------
// Récupère une image ponctuelle auprès de l’ESP32-CAM via HTTP GET sur /capture.
// La fonction remplit le std::vector<uint8_t> avec le flux JPEG reçu.
bool fetchJpeg(std::vector<uint8_t>& jpg) {
  String url = String("http://") + CAM_IP + ":" + String(CAM_PORT) + CAM_CAPTURE_PATH;

  HTTPClient http;
  http.setTimeout(5000);        // timeout global
  http.setConnectTimeout(3000); // timeout connexion TCP
  http.setReuse(false);         // pas de keep-alive (stabilité)

  if (!http.begin(url)) {
    Serial.println("[HTTP] begin() a échoué");
    return false;
  }

  int httpCode = http.GET();
  if (httpCode != HTTP_CODE_OK) {
    Serial.printf("[HTTP] GET a échoué, code: %d\n", httpCode);
    http.end();
    return false;
  }

  // Récupération du flux en mode taille connue (Content-Length) ou chunked
  WiFiClient *stream = http.getStreamPtr();
  int len = http.getSize();
  jpg.clear();
  jpg.reserve(len > 0 ? len : 64 * 1024); // réserve initiale si taille inconnue

  if (len > 0) {
    // Cas Content-Length connu : lire exactement len octets
    jpg.resize(len);
    int readTotal = 0;
    while (http.connected() && readTotal < len) {
      size_t avail = stream->available();
      if (avail) {
        int r = stream->read(&jpg[readTotal], std::min((int)avail, len - readTotal));
        if (r > 0) readTotal += r;
      } else {
        delay(1);
      }
    }
    if (readTotal != len) {
      Serial.printf("[HTTP] lecture incomplète (%d/%d)\n", readTotal, len);
      http.end();
      return false;
    }
  } else {
    // Cas chunked/unknown : lire par blocs jusqu’à fermeture/fin
    const size_t chunk = 2048;
    uint8_t buf[chunk];
    while (http.connected()) {
      size_t avail = stream->available();
      if (avail == 0) {
        if (!stream->connected()) break;
        delay(1);
        continue;
      }
      int r = stream->readBytes(buf, std::min(avail, chunk));
      if (r > 0) jpg.insert(jpg.end(), buf, buf + r);
      else break;
    }
  }
  http.end();
  return !jpg.empty();
}

// ---------- Affichage + capture conditionnelle ----------
// Affiche une UI d’attente une seule fois (quand pas de détection).
void showWaitingUiOnce() {
  if (showingWaitUi) return;
  showingWaitUi = true;

  M5.Display.clear();
  M5.Display.setTextDatum(middle_center);
  M5.Display.setTextSize(2);
  M5.Display.setTextColor(TFT_WHITE, TFT_BLACK);
  M5.Display.drawString("attente detection mouvement",
                        M5.Display.width() / 2,
                        M5.Display.height() / 2);
}

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);
  Serial.begin(115200);

  // Écran : orientation paysage, luminosité max, écran propre
  M5.Display.setRotation(1);
  M5.Display.setBrightness(255);
  M5.Display.clear();

  // Entrée trigger (capteur PIR) avec pull-down interne
  pinMode(PIN_TRIGGER, INPUT_PULLDOWN);

#ifdef BOARD_SDIO_ESP_HOSTED_CLK
  // Si la carte fournit des pins SDIO hôtes, les utiliser
  WiFi.setPins(BOARD_SDIO_ESP_HOSTED_CLK, BOARD_SDIO_ESP_HOSTED_CMD,
               BOARD_SDIO_ESP_HOSTED_D0,  BOARD_SDIO_ESP_HOSTED_D1,
               BOARD_SDIO_ESP_HOSTED_D2,  BOARD_SDIO_ESP_HOSTED_D3,
               BOARD_SDIO_ESP_HOSTED_RESET);
#else
  // Sinon, utiliser la configuration alternative (SDIO2_*)
  WiFi.setPins(SDIO2_CLK, SDIO2_CMD, SDIO2_D0, SDIO2_D1, SDIO2_D2, SDIO2_D3, SDIO2_RST);
#endif

  // Connexion au hotspot Wi-Fi (WPA2-PSK)
  M5.Display.setTextSize(2);
  M5.Display.println("Connexion Wi-Fi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    M5.Display.print(".");
  }
  Serial.println("WiFi OK !");

  // Initialisation microSD + création du répertoire de captures
  beginSD_Tab5_SPI();
  ensureCapturesDir();

  // UI d’attente par défaut
  showWaitingUiOnce();
}

void loop() {
  M5.update();  // Gestion interne des events M5 (tactile, etc.)

  const uint32_t now = millis();
  const bool trigHigh = (digitalRead(PIN_TRIGGER) == HIGH); // état du PIR (via G4)

  if (trigHigh) {
    // Un mouvement a été détecté : forcer l’UI en mode « flux »
    showingWaitUi = false;

    // Récupère et affiche régulièrement une image (rafraîchissement limité par kMinIntervalMs)
    if (now - lastFetchMs >= kMinIntervalMs) {
      lastFetchMs = now;

      std::vector<uint8_t> jpg;
      if (fetchJpeg(jpg)) {
        // Affichage plein écran de l’image JPEG à la taille du display
        M5.Display.drawJpg(jpg.data(), jpg.size(), 0, 0,
                           M5.Display.width(), M5.Display.height());

        // Sauvegarde périodique quand le trigger est maintenu à HIGH
        if (now - lastSaveMs >= kSaveIntervalMs) {
          saveJpegToSD(jpg);
          lastSaveMs = now;
        }
      } else {
        Serial.println("Erreur réception image");
      }
    }
  } else {
    // Pas de mouvement : afficher une UI d’attente (une seule fois)
    showWaitingUiOnce();
  }

  delay(2); // Petite pause pour relâcher le CPU (évite boucle 100% busy)
}
