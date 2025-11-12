README – Motion CAM-RF
Academic project carried out as part of the 5th year of engineering school at the University of Versailles Saint-Quentin-en-Yvelines
Specialization: Embedded Electronic Systems

Wireless motion detection and video transmission system based on ESP32-CAM and M5Stack Tab5.

Project Description:
The Motion CAM-RF project aims to design and document an embedded local surveillance system combining motion detection and real-time video transmission. When motion is detected by the HC-SR501 sensor, the M5Stack Tab5 tablet automatically establishes a Wi-Fi connection with the ESP32-CAM to display the corresponding MJPEG video stream.
This system operates indoors on a local 2.4 GHz Wi-Fi network provided by a smartphone hotspot. The project is part of an academic study focusing on radiofrequency transmission, hardware integration, and embedded development.

System Architecture:
HC-SR501 → M5Stack Tab5 ⇄ Wi-Fi 2.4 GHz ⇄ ESP32-CAM

Component Description:

HC-SR501: Passive infrared (PIR) sensor detecting human motion through thermal variation.
M5Stack Tab5: Central control unit responsible for event handling, display, and network management.
ESP32-CAM: Embedded camera module streaming MJPEG video via Wi-Fi.
Smartphone Hotspot: 2.4 GHz Wi-Fi access point (WPA2-PSK) connecting the Tab5 and the camera.

Technical Specifications:
Wi-Fi Band: 2.4 GHz (IEEE 802.11 b/g/n)
Modulation: OFDM / DSSS
Max RF Power (EIRP): 100 mW (20 dBm)
Typical Range: ~10 m indoors
Display Latency: < 5 s
Power Supply: 5 V DC (USB-C or regulated power source)
Intended Use: Indoor only
Wi-Fi Security: WPA2-PSK
CE Compliance Standard: RED 2014/53/EU

Main Features:
Motion detection via PIR sensor (HC-SR501)
Real-time MJPEG video display on the Tab5
Automatic Wi-Fi access point connection
Automatic reconnection after network loss
Display timeout (30 s after last detection)
Local image storage on Tab5 (microSD card)
Simple and intuitive touchscreen interface

Hardware Components:
HC-SR501: PIR sensor – 5V – 120° field of view – 3 to 7 m range
M5Stack Tab5: Controller & display – ESP32-P4 + ESP32-C6 – 5″ touchscreen – Wi-Fi 6 – integrated SD
ESP32-CAM: Video capture – OV2640 – 2 MP – MJPEG HTTP – 2.4 GHz Wi-Fi
Smartphone Hotspot: Access point – 2.4 GHz Wi-Fi WPA2 – DHCP – dynamic IP

Regulatory Compliance:
The Motion CAM-RF project is based on electronic components compliant with European requirements under the RED 2014/53/EU directive.

Main Standards:
Safety: EN 62368-1:2020 (safety of radio and multimedia equipment)
RF Exposure: EN 62479:2010 / EN 62311:2020 (human exposure assessment)
EMC: EN 301 489-1 / -17 (Wi-Fi / Bluetooth electromagnetic compatibility)
Radio: EN 300 328 V2.2.2 (broadband 2.4 GHz transmission)

Cybersecurity: EN 303 645:2020 (IoT device security)
The ESP32-C6 and ESP32-P4 modules used in the M5Stack Tab5 are CE and FCC pre-certified, as documented by Espressif.

Author:
Project: Motion CAM-RF
Developed by: Brice BENINI
