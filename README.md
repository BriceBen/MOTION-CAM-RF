README – Motion CAM-RF

Academic project carried out as part of the 5th year of engineering
school at the University of Versailles Saint-Quentin-en-Yvelines
Specialization: Embedded Electronic Systems

Wireless motion detection and video transmission system based on
ESP32-CAM and M5Stack Tab5.

Project Description

The Motion CAM-RF project implements an embedded indoor surveillance
system combining PIR-based motion detection with real-time MJPEG video
streaming. When the HC-SR501 sensor detects motion, the M5Stack Tab5
automatically connects to the ESP32-CAM over a 2.4 GHz Wi-Fi hotspot and
displays the camera feed.

This project focuses on RF communication, embedded development, and
hardware integration.

System Architecture

HC-SR501 → M5Stack Tab5 ⇄ Wi-Fi 2.4 GHz ⇄ ESP32-CAM

Components

-   HC-SR501: PIR sensor detecting thermal variations.
-   M5Stack Tab5: Central controller managing detection, video display,
    storage, and network.
-   ESP32-CAM: Camera module providing MJPEG streaming.
-   Smartphone Hotspot: Wi-Fi access point (WPA2-PSK).

Main Features

-   Motion detection via PIR sensor
-   Live MJPEG display on Tab5
-   Automatic Wi-Fi AP connection
-   Auto-reconnect on signal loss
-   30-second display timeout
-   Local JPEG storage on microSD
-   Intuitive touchscreen interface

Usage

1.  Enable smartphone hotspot
2.  Power the Tab5 and ESP32-CAM
3.  On motion detection:
    -   Tab5 opens http://CAM_IP:81/stream
    -   Live video displays
    -   Snapshot saved every 3 seconds
4.  ESP32-CAM Web UI: http://CAM_IP/

Tests and Performance

-   Latency: 1–3 seconds
-   Indoor range: up to 10 m
-   RF power: ≤ 20 dBm
-   Stream rate: 5–15 FPS

Documentation

Full technical documentation is available in /docs.

Regulatory Compliance

Compliant with RED 2014/53/EU through: - Safety: EN 62368-1 - RF
Exposure: EN 62479 / EN 62311 - EMC: EN 301 489-1 / -17 - Radio: EN 300
328 V2.2.2 - Cybersecurity: EN 303 645

The ESP32-C6 and ESP32-P4 modules in the Tab5 are CE and FCC
pre-certified.

Sources & Technical References

-   FCC Test Report – ESP32-C6-MINI-1
-   ESP32-C6-MINI-1 module datasheet
-   ESP32-P4 processor documentation
-   HC-SR501 PIR sensor datasheet
-   Espressif ESP-IDF documentation
-   M5Stack Tab5 technical documentation

Author

Brice BENINI
