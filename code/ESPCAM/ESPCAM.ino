#include "esp_camera.h"
#include <WiFi.h>

// ===================
// Sélection du modèle
// ===================
// Le modèle AI-THINKER (ESP32-CAM le plus courant) possède de la PSRAM,
// indispensable pour des résolutions/qualités JPEG élevées.
#define CAMERA_MODEL_AI_THINKER // Has PSRAM
#include "camera_pins.h"        // Map des broches selon le modèle sélectionné

// ===========================
// Wi-Fi
// ===========================
// Identifiants du point d’accès (hotspot du smartphone).
// La caméra s’y connecte pour être joignable en IP par la Tab5.
const char* ssid     = "Brice's Galaxy S10";
const char* password = "ldv2019!";

// Prototypes (déclarés ailleurs dans l’exemple officiel ESP32-CAM)
// - startCameraServer() lance le serveur HTTP qui expose /stream et /capture
// - setupLedFlash(pin) configure le flash LED si présent
void startCameraServer();
void setupLedFlash(int pin);

void setup() {
  Serial.begin(115200);           // Console série pour logs/debug
  Serial.setDebugOutput(true);    // Active les messages détaillés de certaines libs
  Serial.println();

  // --- Configuration de la caméra ---
  // La structure camera_config_t définit toutes les broches, le timing et le format d’image.
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;   // Canal PWM pour générer l’horloge XCLK
  config.ledc_timer   = LEDC_TIMER_0;     // Minuterie liée au canal précédent

  // Bus parallèle du capteur (lignes de données D0..D7)
  config.pin_d0       = Y2_GPIO_NUM;
  config.pin_d1       = Y3_GPIO_NUM;
  config.pin_d2       = Y4_GPIO_NUM;
  config.pin_d3       = Y5_GPIO_NUM;
  config.pin_d4       = Y6_GPIO_NUM;
  config.pin_d5       = Y7_GPIO_NUM;
  config.pin_d6       = Y8_GPIO_NUM;
  config.pin_d7       = Y9_GPIO_NUM;

  // Signaux de synchro/horloges caméra
  config.pin_xclk     = XCLK_GPIO_NUM;    // Horloge capteur (générée par l’ESP32)
  config.pin_pclk     = PCLK_GPIO_NUM;    // Pixel clock (sortie capteur)
  config.pin_vsync    = VSYNC_GPIO_NUM;   // Sync verticale (début/fin d’image)
  config.pin_href     = HREF_GPIO_NUM;    // Sync horizontale (ligne valide)

  // Bus de configuration I2C (SCCB) du capteur (réglages internes)
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;

  // Gestion d’alimentation/réinitialisation du module caméra (selon carte)
  config.pin_pwdn     = PWDN_GPIO_NUM;
  config.pin_reset    = RESET_GPIO_NUM;

  // Fréquence d’horloge du capteur : 20 MHz recommandé pour OV2640
  config.xclk_freq_hz = 20000000;

  // Format de sortie : JPEG compressé (plus léger à envoyer en réseau)
  config.pixel_format = PIXFORMAT_JPEG;

  // Emplacement des frame buffers : en PSRAM (indispensable pour HD)
  config.fb_location  = CAMERA_FB_IN_PSRAM;

  // Mode de capture : toujours prendre le dernier frame disponible (latence moindre)
  config.grab_mode    = CAMERA_GRAB_LATEST;

  // Choix dynamique de la résolution/qualité selon présence PSRAM
  if (psramFound()) {
    config.frame_size   = FRAMESIZE_HD;   // 1280x720 (16:9) → bon compromis qualité/débit
    config.jpeg_quality = 10;             // Qualité JPEG (0=meilleure, 63=moindre). 10 ≈ bonne qualité.
    config.fb_count     = 2;              // Deux buffers pour pipeline fluide (capture/encodage/serveur)
  } else {
    // Sans PSRAM : réduire la résolution et le nombre de buffers
    config.frame_size   = FRAMESIZE_SVGA; // 800x600
    config.jpeg_quality = 12;             // Légère baisse de qualité pour ménager la RAM
    config.fb_count     = 1;
  }

  // --- Initialisation caméra ---
  // Tente d’initialiser le capteur avec la config ci-dessus.
  // Si erreur, afficher le code (0x200xx, etc.) et s’arrêter.
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x\n", err);
    return;
  }

  // Accès à l’objet capteur pour affiner des paramètres (miroir, flip, qualité…)
  sensor_t *s = esp_camera_sensor_get();

  // Orientation & réglages de base (adapter selon montage)
  s->set_hmirror(s, 0);    // 1 = miroir horizontal (utile si l’image est inversée gauche/droite)
  s->set_vflip(s, 0);      // 1 = flip vertical (utile si la caméra est montée à l’envers)
  s->set_brightness(s, 1); // Légère augmentation de luminosité (+1)
  s->set_saturation(s, -1);// Saturation -1 (couleurs un peu moins vives pour réduire artefacts JPEG)
  s->set_quality(s, 10);   // Cohérent avec config.jpeg_quality

  // --- Wi-Fi ---
  // Connexion au hotspot (mode station). Le DHCP donnera une IP à la caméra.
  WiFi.begin(ssid, password);
  WiFi.setSleep(false);    // Désactive la mise en veille Wi-Fi pour éviter des micro-coupures du flux

  Serial.print("Connexion Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWi-Fi connecté !");
  Serial.print("Adresse IP : ");
  Serial.println(WiFi.localIP()); // À reporter dans le client (Tab5) pour consommer le flux

  // Si la carte possède une LED flash (définie dans camera_pins.h), on la prépare.
#if defined(LED_GPIO_NUM)
  setupLedFlash(LED_GPIO_NUM);
#endif

  // --- Démarrage du serveur HTTP ---
  // Lance le serveur web qui expose :
  //   - /stream   : flux MJPEG (multipart)
  //   - /capture  : capture JPEG ponctuelle
  startCameraServer();

  Serial.println("Serveur caméra prêt !");
  Serial.print("Utilisez l'URL suivante dans votre client M5 : http://");
  Serial.print(WiFi.localIP());
  Serial.println("/capture"); // Pour un test rapide dans un navigateur
}

void loop() {
  // Boucle principale passive : tout le travail est géré par les callbacks HTTP du serveur.
  // On laisse un délai pour éviter d’occuper inutilement le CPU.
  delay(10000);
}
