# RADIO ELECTRONIC SYSTEM SPECIFICATIONS

## PROJECT INFORMATION
- Project Name: MOTION CAM-RF
- Designer: Brice BENINI
- Date: 2025-10-31
- Version: 1.0

## 1. GENERAL SPECIFICATIONS
- System Type: Half-duplex bidirectional Wi-Fi transceiver
- Device Category: Short Range Device (SRD)
- CEPT Annex: Annex 1
- Station Type: Wi-Fi Station (End-user terminal)
- Application: Commercial
- Intended Use: Short-range Wi-Fi communication for local video transmission and motion detection monitoring
- Main Target Market: France
- Operating Mode: Duplex

## 2. FREQUENCY SPECIFICATIONS
- Frequency Range: 2400 MHz – 2483.5 MHz
- Center Frequency: 2 442 MHz
- Channel Spacing: 5 MHz
- Bandwidth: 20 MHz
- EFIS Verified: Yes
- EFIS Notes: Bande vérifiée dans la base EFIS pour usage Wi-Fi / ISM (2400–2483.5 MHz, 100 mW EIRP)

## 3. TRANSMITTER SPECIFICATIONS
- Output Power: 100mW
- Frequency Stability: 20ppm
- Spurious Emission: <-36 dBm 
- Modulation: OFDM / OFDMA (Orthogonal Frequency Division Multiplexing / Multiple Access)

## 4. RECEIVER SPECIFICATIONS
- Sensitivity: −90 dBm
- Selectivity: >35 dB
- Noise Figure: ≤ 7 dB
- Dynamic Range: 80 dB

## 5. ANTENNA SPECIFICATIONS
- Implementation: PCB Printed Antenna
- Type: Monopôle
- Gain: 2 dBi
- VSWR: ≤ 2.0 : 1
- Polarization: Linéaire

## 6. FEATURES & CHARACTERISTICS
- Input Voltage: 5V
- Power Consumption: RX : 80 mA TX : 380mA
- Operating Temperature: 0 to 40°c 
- Humidity: 60%RH
- Dimensions: 13.2 × 12.5 × 2.4 mm
- Weight: 0.2Kg

## 7. RF CHIPSET
- Manufacturer: Espressif
- Model: ESP32-C6-MINI-1
- Details: 
- Application Note: 

## 10. CYBERSECURITY
- Applicable Standards: ETSI EN 303 645

## 11. STANDARDS & CERTIFICATIONS
- Genorma Search: https://genorma.com/en/project/advanced-search?keywords=Commercial&status%5B0%5D=PUBLISHED&status%5B2%5D=WITHDRAWN&operatorCommittee=1&operatorStdType=3&listMode=DEFAULT
- Market Access (360compliance): https://360compliance.co/marketaccess/france/
- IEC Safety: IEC 62368-1:2018
- IEC EMC: IEC 61000-6-1:2016 / IEC 61000-6-3:2020
- CENELEC Safety: EN 62368-1:2020
- CENELEC EMC: EN 301 489-1:2022 / EN 301 489-17:2022
- ETSI: EN 300 328 V2.2.2 (2019-07)
- IEEE: IEEE 802.11
- 3GPP: Not applicable (Wi-Fi system)
- Certifications: CE, FCC, IC, UKCA, RCM

## 12. SAR SAFETY
- Foreseeable Use: Not specified
- SAR Limit: Other
- Notes: usage sans contact humain direct, distance ≥ 20 cm, dispositif fixe, utilisation en intérieur, non porté.

## 13. USE CASES
- Use Case 1: Surveillance intérieure déclenchée par mouvement: Un système de surveillance intérieure détecte un mouvement grâce à un capteur infrarouge passif (PIR) et affiche automatiquement le flux vidéo d’une caméra ESP32-CAM via le réseau Wi-Fi local (2,4 GHz).
La tablette M5Stack Tab5 sert d’interface d’affichage et de commande.
Ce cas d’usage illustre une solution simple de vidéosurveillance locale pour un appartement, un bureau ou une salle de laboratoire.
- Use Case 2: Détection de présence domestique : Dans un environnement résidentiel, le système détecte la présence d’une personne dans une pièce (entrée, couloir, salon) et affiche en temps réel le flux vidéo associé sur l’écran de la Tab5.
La communication entre les modules s’effectue sur un réseau Wi-Fi fermé (partage de connexion du smartphone).
Le dispositif garantit une latence faible (< 5 s) et le respect de la confidentialité, puisque les données ne quittent pas le réseau local.
- Use Case 3: 

## 14. FUNCTIONAL REQUIREMENTS

### Requirement 1: Détection de mouvement
- Sub-requirement 1.1: Le capteur PIR (HC-SR501) doit détecter toute variation thermique correspondant à une présence humaine.
- Sub-requirement 1.2: Le signal de détection doit être transmis à la M5Stack Tab5 via une entrée GPIO, avec un anti-rebond logiciel de 500 ms.

### Requirement 2: Affichage du flux vidéo
- Sub-requirement 2.1: Lorsqu’un mouvement est détecté, la Tab5 doit afficher automatiquement le flux vidéo MJPEG issu de la caméra ESP32-CAM.
- Sub-requirement 2.2: Le flux doit apparaître en plein écran, avec une latence maximale de 5 s entre la détection et l’affichage.

### Requirement 3: Communication Wi-Fi
- Sub-requirement 3.1: Le système doit utiliser la bande 2,4 GHz (Wi-Fi IEEE 802.11 b/g/n) via un point d’accès local (hotspot smartphone).
- Sub-requirement 3.2: Le protocole de sécurité doit être WPA2-PSK, et la connexion doit se rétablir automatiquement après une perte de signal.