******************************************README – Motion CAM-RF************************************

**************Projet scolaire réalisé dans le cadre de la 5ᵉ année d’école d’ingénieur**************
**************à l’Université de Versailles Saint-Quentin-en-Yvelines********************************
**************spécialité Systèmes Électroniques Embarqués*******************************************


Système de détection de mouvement et de transmission vidéo sans fil basé sur ESP32-CAM et M5Stack Tab5.

Description du projet:

Le projet Motion CAM-RF vise à concevoir et documenter un système embarqué de surveillance locale combinant détection de mouvement et transmission vidéo en temps réel. Lorsqu’un mouvement est détecté par le capteur HC-SR501, la tablette M5Stack Tab5 établit automatiquement une connexion Wi-Fi avec la caméra ESP32-CAM afin d’afficher le flux vidéo MJPEG correspondant.

Ce système fonctionne en environnement intérieur, sur un réseau Wi-Fi local (2,4 GHz) fourni par un smartphone en mode hotspot. Le projet s’inscrit dans une démarche académique d’étude de la transmission radiofréquence, de l’intégration matérielle et du développement embarqué.

Architecture du système:
HC-SR501 → M5Stack Tab5 ⇄ Wi-Fi 2.4 GHz ⇄ ESP32-CAM

Description des éléments :
- HC-SR501 : capteur infrarouge passif (PIR) détectant le mouvement humain par variation thermique.
- M5Stack Tab5 : unité centrale de commande, traitement de l’événement, affichage et gestion réseau.
- ESP32-CAM : module caméra embarqué diffusant un flux vidéo MJPEG via Wi-Fi.
- Hotspot Smartphone : point d’accès Wi-Fi 2,4 GHz (WPA2-PSK) reliant la Tab5 et la caméra.

Spécifications techniques:
- Bande Wi-Fi : 2.4 GHz (IEEE 802.11 b/g/n)
- Modulation : OFDM / DSSS
- Puissance RF max (EIRP) : 100 mW (20 dBm)
- Portée typique : ~10 m en intérieur
- Latence d’affichage : < 5 s
- Alimentation : 5 V DC (USB-C ou alimentation stabilisée)
- Usage prévu : Intérieur uniquement
- Sécurité Wi-Fi : WPA2-PSK
- Norme de conformité CE : RED 2014/53/EU

Fonctionnalités principales:
- Détection de mouvement par capteur PIR (HC-SR501)
- Affichage en temps réel du flux MJPEG sur la Tab5
- Connexion automatique au point d’accès Wi-Fi
- Reconnexion automatique après coupure réseau
- Temporisation d’affichage (30 s après la dernière détection)
- Enregistrement local des images sur la Tab5 (carte microSD)
- Interface tactile simple et intuitive

Composants matériels:
- HC-SR501 : Capteur PIR – 5V – champ 120° – portée 3 à 7 m
- M5Stack Tab5 : Contrôleur et affichage – ESP32-P4 + ESP32-C6 – écran tactile 5″ – Wi-Fi 6 – SD intégrée
- ESP32-CAM : Capture vidéo – OV2640 – 2 Mpx – MJPEG HTTP – Wi-Fi 2.4 GHz
- Hotspot Smartphone : Point d’accès – Wi-Fi 2.4 GHz WPA2 – DHCP – IP dynamique

Conformité réglementaire

- Le projet Motion CAM-RF s’appuie sur des composants électroniques conformes aux exigences européennes de la directive RED 2014/53/EU.

Normes principales :
- Sécurité : EN 62368-1:2020 (sécurité des équipements radio et multimédia)
- Exposition RF : EN 62479:2010 / EN 62311:2020 (évaluation de l’exposition humaine)
- CEM (EMC) : EN 301 489-1 / -17 (compatibilité électromagnétique Wi-Fi / Bluetooth)
- Radio : EN 300 328 V2.2.2 (transmission 2.4 GHz large bande)
- Cybersécurité : EN 303 645:2020 (sécurité des objets connectés IoT)
- Les modules ESP32-C6 et ESP32-P4 utilisés dans la M5Stack Tab5 sont pré-certifiés CE et FCC, conformément à la documentation Espressif.

Auteur:
- Projet Motion CAM-RF
- Réalisé par : Brice BENINI
