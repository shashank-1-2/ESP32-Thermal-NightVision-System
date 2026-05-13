# ESP32-Thermal-NightVision-System
ESP32-CAM Night Vision Tracker A portable spy cam that sees in the dark, tracks heat, and streams video over WiFi. Uses AI to follow movement or people. Great for night security or wildlife watching.

# ESP32 Thermal Night Vision System

## Overview
Portable surveillance system combining:
- ESP32-CAM
- MLX90640 thermal sensor
- IR night vision
- WiFi streaming

## Features
- Night vision surveillance
- Thermal hotspot detection
- Wireless browser streaming
- Portable battery-powered system
- AI-ready architecture

## Hardware Used
- ESP32-CAM
- MLX90640
- IR LEDs
- Li-ion Battery Pack

## System Architecture
MLX90640 Thermal Sensor  ─┐
                          │
                          ├──> ESP32-CAM ───> WiFi Streaming ───> Browser
IR LEDs -----------------┘
                          │
                          └──> Servo Motor Control

## Working Principle
-The ESP32-CAM captures real-time video footage.
-IR LEDs provide illumination for night vision in dark environments.
-The MLX90640 thermal sensor continuously scans the surroundings and measures temperature distribution.
-Thermal hotspots are identified and processed by the ESP32.
-Live video and thermal data are streamed wirelessly through WiFi to a web browser.
-The system operates on a portable battery supply, enabling standalone deployment.

## Installation
-Arduino IDE setup
-Add ESP32 board support:
  Open Preferences
  Add ESP32 board URL:
    https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json

#Install the following libraries from Arduino Library Manager:
-esp32-camera
-WiFi
-Adafruit MLX90640
-Adafruit BusIO
-ESPAsyncWebServer
-AsyncTCP

## Results
Add screenshots/videos.

## Team Members
- Shashank Kumar
- Tanish Ranjan
- Naveen Modi

## Future Scope
- TensorFlow Lite integration for AI-based object detection
- Dedicated mobile application
- Cloud storage and remote monitoring
- Smart alert notifications
- Human/animal recognition
- Automatic target tracking
- Advanced thermal analytics

## Real-Life Applications
- Smart security systems
- Night surveillance
- Wildlife monitoring
- Industrial heat monitoring
- Search & rescue operations
- Fire hazard detection
- Military and defense applications
