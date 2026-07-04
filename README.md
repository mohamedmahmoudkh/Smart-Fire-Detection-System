# 🔥 Smart Fire Detection System

An IoT-based fire alarm built on the **ESP32** microcontroller. It continuously monitors a digital flame sensor and, upon detecting fire, triggers a local buzzer alarm **and** sends a real-time WhatsApp alert to a designated phone number via the CallMeBot API.

## ✨ Features

| Feature | Description |
|---|---|
| 🔥 Real-Time Detection | Continuous polling of the flame sensor every loop cycle |
| 🚨 Dual Alert Mechanism | Local buzzer alarm + WhatsApp push notification |
| ✅ Startup Confirmation | Sends a "Your system is ready!" message on boot |
| 🌐 HTTP API Integration | Uses the CallMeBot WhatsApp API over HTTPS |
| 🖥️ Serial Debugging | 115200 baud serial output for diagnostics |
| 📶 Wi-Fi Connectivity | Automatic connection with status feedback |

## 🧰 Hardware Components

| Component | Details |
|---|---|
| Microcontroller | ESP32 (dual-core, 240 MHz, built-in Wi-Fi & Bluetooth) |
| Flame Sensor | Digital IR Flame Sensor Module (DO output) |
| Buzzer | Active Buzzer Module (5V compatible) |
| Power Supply | USB 5V via ESP32 dev board |
| Connectivity | Wi-Fi 802.11 b/g/n (2.4 GHz) |

### Pin Assignment

| Pin | Connection |
|---|---|
| GPIO 32 | Flame Sensor (INPUT) — digital reading from DO pin |
| GPIO 33 | Buzzer (OUTPUT) — HIGH = alarm ON, LOW = alarm OFF |
| GND | Common ground for all modules |
| 3.3V / 5V | Power supply for sensor and buzzer modules |

### Wiring

```
Flame Sensor DO  → GPIO 32
Flame Sensor VCC → 3.3V
Flame Sensor GND → GND
Buzzer (+)       → GPIO 33
Buzzer (-)       → GND
```

## 🏗️ Software Architecture

**Libraries used:**
- `WiFi.h` — ESP32 built-in, manages Wi-Fi connection
- `HTTPClient.h` — ESP32 built-in, sends HTTP requests
- `UrlEncode.h` — third-party, URL-encodes the WhatsApp message text

**Program flow:**
- **`setup()`** — initializes serial (115200 baud), configures GPIO pins, connects to Wi-Fi, and sends a startup confirmation message via WhatsApp.
- **`loop()`** — reads the flame sensor; if fire is detected, turns the buzzer ON and sends a WhatsApp alert; otherwise keeps the buzzer OFF.
- **`sendAlert(message)`** — builds the CallMeBot API URL, sends an HTTPS POST request, and logs the response code to Serial.

## 📲 CallMeBot WhatsApp API

The project uses the free [CallMeBot](https://www.callmebot.com/) WhatsApp API — no WhatsApp Business account required.

**Endpoint:**
```
https://api.callmebot.com/whatsapp.php?phone={number}&apikey={key}&text={message}
```

**Getting your API key:**
1. Add `+34 644 59 21 83` to your WhatsApp contacts.
2. Send the message: *"I allow callmebot to send me messages"*.
3. The bot replies with your personal API key within seconds.
4. Use that key as the `apiKey` value in the sketch.

> ⚠️ **Security note:** Never commit your real API key, Wi-Fi password, or phone number to a public repository. Use a separate config file or environment variables for production deployments.

## ⚙️ Setup & Deployment

1. **Install Arduino IDE & ESP32 board package**
   - Download [Arduino IDE](https://www.arduino.cc/en/software).
   - In `File > Preferences`, add the ESP32 board URL:
     ```
     https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
     ```
   - In `Tools > Board Manager`, search "esp32" and install the Espressif package.

2. **Install required library**
   - `Sketch > Include Library > Manage Libraries` → search "UrlEncode" (by Masayuki Sugahara) → Install.

3. **Configure your credentials** in the sketch:
   ```cpp
   const char* ssid = "YOUR_SSID";
   const char* password = "YOUR_PASSWORD";
   String phoneNumber = "+201XXXXXXXXX"; // E.164 format
   String apiKey = "XXXXXXX";            // from CallMeBot
   ```

4. **Upload the sketch**
   - Connect the ESP32 via USB, select **ESP32 Dev Module** and the correct COM port, then click Upload.

5. **Verify operation**
   - Open the Serial Monitor at 115200 baud.
   - Confirm Wi-Fi connects and an IP address is printed.
   - Check WhatsApp for the "Your system is ready!" message.
   - Bring a flame near the sensor — the buzzer should sound and a WhatsApp alert should arrive.

## 🐛 Troubleshooting

| Issue | Fix |
|---|---|
| Wi-Fi not connecting | Verify SSID/password, ensure 2.4 GHz band, move closer to router |
| No WhatsApp message | Re-register the API key with the CallMeBot bot; check phone number format |
| HTTP error 429 | Too many alerts sent — add a `delay()` or use the rate-limiting fix below |
| Buzzer always ON | Check sensor wiring and the sensitivity trim potentiometer |
| Buzzer never ON | Print `sensorValue` over Serial to check the DO pin level |
| Upload fails | Hold the BOOT button during upload; check driver installation |

## 🚧 Known Limitations & Future Improvements

**Current limitations:**
- Sends a WhatsApp message on every loop iteration while fire is detected, which can hit API rate limits (429 errors).
- No Wi-Fi reconnection logic if the connection drops mid-operation.
- Wi-Fi/API credentials are hardcoded in the source file.
- Single sensor only — no redundancy or multi-zone detection.

**Suggested improvements:**
- Add an `alertSent` flag so only one WhatsApp message is sent per fire event (see fix below).
- Implement Wi-Fi reconnection with `WiFi.reconnect()`.
- Store credentials via EEPROM or Wi-Fi provisioning (SmartConfig / BLE).
- Add an OLED display for local real-time status.
- Integrate a DHT22 temperature/humidity sensor for richer alerts.
- Add Telegram or email as backup notification channels.
- Implement MQTT for centralized IoT dashboard monitoring.

**Rate-limiting fix (send one alert per fire event):**
```cpp
bool alertSent = false;

void loop() {
  bool sensorValue = digitalRead(Sensor);
  if (sensorValue == 1 && !alertSent) {
    digitalWrite(Buzzer, HIGH);
    sendAlert("Warning! Fire Detected...");
    alertSent = true;
  } else if (sensorValue == 0) {
    digitalWrite(Buzzer, LOW);
    alertSent = false; // Reset when fire clears
  }
}
```

## 🧱 Technology Stack

| Component | Detail |
|---|---|
| Microcontroller | ESP32 (Espressif Systems) |
| Language | Arduino C++ (C++11) |
| IDE | Arduino IDE 2.x |
| Networking | HTTPS over Wi-Fi (TLS 1.2) |
| Notification API | CallMeBot WhatsApp API (REST) |
| Libraries | `WiFi.h`, `HTTPClient.h`, `UrlEncode.h` |
| Serial Baud Rate | 115200 |

## 📁 Repository Structure

```
.
├── fire_detectionusingwifi.ino                # ESP32 Arduino sketch — sensor logic, Wi-Fi, WhatsApp alerts
├── SmartFireDetection_Presentation.pptx       # Project presentation slides
├── SmartFireDetection_Documentation.docx      # Full technical documentation
└── README.md
```

## ⚠️ Disclaimer

This is a prototype/educational project. Always test in a controlled environment before deploying in any real fire-safety scenario — it is not a certified fire-safety device and should not replace professional smoke/fire detection systems.
