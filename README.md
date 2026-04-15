# 🌡️ Smart Climate Monitor

**Author:** Sofia Lorin Pardillo  
**Course:** BSICE - 3  
**Activity:** Finals Quiz Equivalent

---

## 📌 What It Is

The Smart Climate Monitor is a real-time environmental 
monitoring device built using the ESP32-S3 microcontroller. 
It reads temperature and humidity data from an AHT30 sensor 
and displays the information on an OLED screen. The user can 
navigate through different screens using 2 push buttons.

---

## 🎯 What It Is For

This project is designed to monitor room comfort levels 
in real time. It is useful in:

- **Classrooms** — to check if the room is too hot or 
too humid for learning
- **Bedrooms** — to ensure sleeping conditions are comfortable
- **Laboratories** — to monitor environmental conditions 
during experiments
- **General use** — anywhere you need to track temperature 
and humidity over time

---

## 🔧 Components Used

| Component | Purpose |
|---|---|
| ESP32-S3 | Main microcontroller |
| AHT30 | Temperature and humidity sensor |
| OLED Display (SSD1306 128x64) | Displays readings |
| Push Button Yellow (GPIO 21) | Cycles through screens |
| Push Button Red (GPIO 5) | Toggles °C / °F |
| Breadboard | Platform for connecting components |
| Jumper Wires | Connects components together |

---

## 🔌 Wiring Guide

### AHT30 and OLED — share the same I2C bus

| Pin | ESP32-S3 |
|---|---|
| VCC | 3.3V |
| GND | GND |
| SDA | GPIO 8 |
| SCL | GPIO 9 |

### Push Buttons

| Button | GPIO | Function |
|---|---|---|
| Yellow | GPIO 21 | Next Screen |
| Red | GPIO 5 | Toggle °C / °F |

---

## 📡 How I2C Works in This Project

Both the AHT30 and OLED share only **2 wires**:

- **SDA (GPIO 8)** — carries the data
- **SCL (GPIO 9)** — keeps communication in sync

They do not conflict because each has a unique address:

| Device | I2C Address |
|---|---|
| OLED Display | `0x3C` |
| AHT30 Sensor | `0x38` |

The ESP32 acts as the **master** and calls each device 
by its address — like a teacher calling students by name. 
They never talk at the same time.

---

## 🖥️ How It Works

### On Startup
When powered on the OLED shows a splash screen with the 
project name, version, author, and course for 2.5 seconds. 
Then it begins reading from the AHT30 and displaying live data.

### 5 Screens

| Screen | What It Shows |
|---|---|
| Screen 1 | Temperature (°C or °F) |
| Screen 2 | Humidity (%) |
| Screen 3 | Heat Index (°C or °F) |
| Screen 4 | Comfort Status + raw readings |
| Screen 5 | Min / Max recorded values |

### Button Functions

| Button | Function |
|---|---|
| Yellow (GPIO 21) | Cycles to next screen, loops back after Screen 5 |
| Red (GPIO 5) | Toggles between Celsius and Fahrenheit |

### Comfort Status Logic

| Condition | Status |
|---|---|
| Temperature > 35°C | Too Hot! |
| Temperature < 18°C | Too Cold! |
| Humidity > 70% | Too Humid! |
| Humidity < 30% | Too Dry! |
| All normal | Comfortable :) |

### Min / Max Recording
The device automatically tracks the lowest and highest 
temperature and humidity readings since power on. 
Records reset every time the device restarts.

### Heat Index
Calculated using the **Rothfusz regression equation** — 
a scientific formula used by meteorologists to calculate 
what temperature actually feels like to the human body 
based on both temperature and humidity.

---

## 📚 Libraries Required

Install via Arduino IDE Library Manager:

- `Adafruit AHTX0`
- `Adafruit SSD1306`
- `Adafruit GFX Library`
- `Adafruit BusIO`

---

## ⚙️ Arduino IDE Settings

| Setting | Value |
|---|---|
| Board | ESP32S3 Dev Module |
| Port | COM12 |
| USB CDC On Boot | Enabled |
| Upload Speed | 921600 |
| Flash Size | 4MB |

---

## 🚀 How to Run

1. Install all required libraries
2. Select **ESP32S3 Dev Module** as the board
3. Select correct COM port
4. Upload the code
5. OLED shows splash screen then starts displaying readings
6. Press **Yellow button** to cycle through screens
7. Press **Red button** to toggle °C / °F

---

## 🔮 Possible Improvements

- **DS3231 RTC** — show real time date and time
- **LED indicators** — Green, Yellow, Red for comfort levels
- **WiFi Dashboard** — monitor readings from phone or laptop
- **7 Segment Display** — show humidity simultaneously
- **Data logging** — save readings to SD card
- **Bluetooth** — send readings to a mobile app

---


*Smart Climate Monitor v3.0 — Sofia Lorin Pardillo, BSICE-3*
