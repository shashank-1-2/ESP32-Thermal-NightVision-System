# 🔭 ESP32 Thermal Night Vision System

<div align="center">

![GitHub stars](https://img.shields.io/github/stars/shashank-1-2/ESP32-Thermal-NightVision-System?style=social)
![GitHub forks](https://img.shields.io/github/forks/shashank-1-2/ESP32-Thermal-NightVision-System?style=social)
![License](https://img.shields.io/badge/license-MIT-blue.svg)
![Platform](https://img.shields.io/badge/platform-ESP32-red.svg)
![Status](https://img.shields.io/badge/status-Active-brightgreen.svg)

**A portable spy cam that sees in the dark, tracks heat, and streams live video over WiFi.**
Built with ESP32-CAM + MLX90640 thermal sensor for night security and wildlife monitoring.

---

<table>
  <tr>
    <td align="center">
      <img src="images/Screenshot_20260510_220645.jpg.jpeg" width="200" height="350" alt="Thermal View"/><br/>
      <b>Thermal Heatmap</b>
    </td>
    <td align="center">
      <img src="images/Screenshot_20260510_222355.jpg.jpeg" width="200" height="350" alt="Night Vision"/><br/>
      <b>Night Vision Feed</b>
    </td>
    <td align="center">
      <img src="images/Screenshot_20260510_222854.jpg.jpeg" width="200" height="350" alt="Hardware"/><br/>
      <b>Hardware Setup</b>
    </td>
  </tr>
</table>

</div>

---

## 📋 Table of Contents
- [Overview](#-overview)
- [Features](#-features)
- [Hardware](#-hardware-used)
- [System Architecture](#-system-architecture)
- [Installation](#-installation)
- [Working Principle](#-working-principle)
- [Results](#-results)
- [Future Scope](#-future-scope)
- [Applications](#-real-life-applications)
- [Team](#-team)

---

## 🔍 Overview

The **ESP32 Thermal Night Vision System** is a portable, WiFi-enabled surveillance system that combines real-time video streaming with thermal imaging. It uses an ESP32-CAM module for night vision footage and an MLX90640 32×24 thermal sensor to detect heat signatures — all streamed wirelessly to any web browser with no app required.

Designed for security, wildlife monitoring, and industrial inspection, the system is compact, battery-powered, and AI-ready for future upgrades.

---

## ✨ Features

| Feature | Description |
|---|---|
| 🌙 **Night Vision** | IR LED illumination for complete darkness operation |
| 🌡️ **Thermal Detection** | MLX90640 32×24 pixel heat map with live color display |
| 📡 **WiFi Streaming** | Live video and thermal data via web browser — no app needed |
| 🔋 **Portable** | Li-ion battery powered for standalone deployment |
| 🎨 **Ironbow Colormap** | Professional thermal visualization (black→purple→red→white) |
| 🔧 **AI-Ready** | Architecture designed for TensorFlow Lite integration |
| 📊 **Live Stats** | Real-time min/max/ambient temperature display |
| 🖥️ **Web Interface** | Clean browser-based dashboard with Start/Stop controls |

---

## 🔩 Hardware Used

| Component | Specification | Purpose |
|---|---|---|
| **ESP32-CAM** | AI Thinker, OV2640 camera | Video capture + WiFi |
| **ESP32 DevKit V1** | DOIT ESP32, 240MHz | Thermal processing |
| **MLX90640** | 32×24 pixels, ±1°C accuracy | Thermal imaging |
| **IR LEDs** | 850nm wavelength | Night vision illumination |
| **Li-ion Battery Pack** | 3.7V / 5V output | Portable power supply |
| **HW-381 MB Board** | CH340 USB programmer | ESP32-CAM programming |

---

## 🏗️ System Architecture

```
┌─────────────────┐     I2C      ┌──────────────┐
│  MLX90640       │─────────────>│              │
│  Thermal Sensor │              │   ESP32      │──> Serial ──> Laptop
│  (32×24 pixels) │              │   DevKit     │
└─────────────────┘              └──────────────┘

┌─────────────────┐              ┌──────────────┐
│  IR LEDs        │─────────────>│   ESP32-CAM  │──> WiFi ──> Browser
│  Night Vision   │              │   + OV2640   │
└─────────────────┘              └──────────────┘
                                        │
                                   WiFi Stream
                                        ▼
                              ┌─────────────────┐
                              │  Web Browser    │
                              │  Live Dashboard │
                              │  Thermal + Video│
                              └─────────────────┘
```

---

## ⚙️ Installation

### Step 1 — Arduino IDE Setup
1. Open Arduino IDE
2. Go to **File > Preferences**
3. Add ESP32 board URL:
```
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
```
4. Go to **Tools > Board > Board Manager**
5. Search **"esp32"** → Install **esp32 by Espressif Systems v3.x.x**

### Step 2 — Install Libraries
Open **Sketch > Include Library > Manage Libraries** and install:

| Library | Author | Purpose |
|---|---|---|
| Adafruit MLX90640 | Adafruit | Thermal sensor driver |
| Adafruit BusIO | Adafruit | I2C/SPI abstraction |
| WiFi | Espressif | WiFi connectivity |
| WebServer | Espressif | Web interface |

### Step 3 — Board Configuration

**For ESP32-CAM:**

| Setting | Value |
|---|---|
| Board | AI Thinker ESP32-CAM |
| Upload Speed | 115200 |
| Flash Frequency | 80MHz |
| Partition Scheme | Huge APP (3MB No OTA) |

**For ESP32 DevKit (Thermal):**

| Setting | Value |
|---|---|
| Board | DOIT ESP32 DEVKIT V1 |
| Upload Speed | 921600 |
| Partition Scheme | Default |

### Step 4 — Wiring

**MLX90640 → ESP32 DevKit:**
```
MLX90640 VIN  ──> ESP32 3.3V
MLX90640 GND  ──> ESP32 GND
MLX90640 SDA  ──> ESP32 GPIO21
MLX90640 SCL  ──> ESP32 GPIO22
```

**ESP32-CAM Power:**
```
ESP32 VIN ──> ESP32-CAM 5V
ESP32 GND ──> ESP32-CAM GND
```

### Step 5 — Configure WiFi
In the sketch update your credentials:
```cpp
const char* ssid     = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";
```

### Step 6 — Flash & Run
1. Upload code to ESP32
2. Open Serial Monitor at **115200 baud**
3. Press **EN/RST** button
4. Note the IP address shown
5. Open IP in browser → Live dashboard!

---

## ⚡ Working Principle

1. **ESP32-CAM** captures real-time video via OV2640 camera module
2. **IR LEDs** illuminate the scene in complete darkness (invisible to human eye)
3. **MLX90640** continuously scans surroundings measuring 768 temperature points
4. ESP32 processes thermal data — applies smoothing, Gaussian blur, and ironbow colormap
5. Live video and thermal heatmap are streamed wirelessly via WiFi
6. Web browser displays real-time dashboard with min/max/ambient temperature stats
7. System runs on portable Li-ion battery for standalone deployment

---

## 📊 Results

> 📸 Add your screenshots and demo videos here!

**Performance Metrics:**

| Metric | Value |
|---|---|
| Thermal Resolution | 32 × 24 pixels |
| Temperature Range | -40°C to +300°C |
| Accuracy | ±1°C |
| Refresh Rate | Up to 16 Hz |
| Video Stream | QVGA (320×240) |
| WiFi Range | ~50 meters |
| Battery Life | ~3-4 hours |

---

## 🚀 Future Scope

- [ ] **TensorFlow Lite** — AI-based human/animal detection
- [ ] **Mobile App** — Dedicated iOS/Android application
- [ ] **Cloud Storage** — Remote monitoring and recording
- [ ] **Smart Alerts** — Push notifications on heat detection
- [ ] **Servo Tracking** — Automatic target following
- [ ] **Higher Resolution** — Upgrade to FLIR Lepton (80×60)
- [ ] **Multi-Camera** — Synchronized multi-angle coverage
- [ ] **Advanced Analytics** — Temperature trend graphs

---

## 🌍 Real-Life Applications

| Domain | Use Case |
|---|---|
| 🔒 **Security** | Smart home and perimeter surveillance |
| 🦁 **Wildlife** | Nocturnal animal monitoring |
| 🏭 **Industrial** | Electrical fault and heat leak detection |
| 🔥 **Fire Safety** | Early fire hazard identification |
| 🚑 **Search & Rescue** | Locating people in smoke or darkness |
| 🪖 **Defense** | Night patrol and threat detection |
| 🏥 **Medical** | Fever screening and thermal profiling |

---

## 👥 Team

| Name | Role |
|---|---|
| **Shashank Kumar** | Hardware & ESP32-CAM Integration |
| **Tanish Ranjan** | Thermal Sensor & Data Processing |
| **Naveen Modi** | Web Interface & WiFi Streaming |

---

## 📄 License

This project is licensed under the MIT License — see the [LICENSE](LICENSE) file for details.

---

<div align="center">
Made with ❤️ using ESP32 + MLX90640
<br><br>
⭐ Star this repo if you found it helpful!
</div>
