# ESP32 Temperature & Humidity Monitor 🌡️💧

A low-power ESP32 application that reads temperature and humidity data from a DHT22 sensor and sends it via MQTT to a central broker. The device uses deep sleep mode to maximize battery life, waking up every minute to take measurements.

## Features ✨

- 📊 Reads temperature and humidity from DHT22 sensor
- 📡 Sends data via MQTT in JSON format
- 🔋 Ultra-low power consumption using ESP32 deep sleep
- 🌐 WiFi connectivity with automatic reconnection
- 🔄 Automatic MQTT reconnection handling
- ⏰ Configurable measurement intervals (default: 60 seconds)

## Hardware Requirements 🔧

- ESP32 development board
- DHT22 (AM2302) temperature and humidity sensor
- Jumper wires
- Breadboard (optional)
- Power source (USB or battery)

## Wiring Diagram 📐

```
DHT22 Sensor    ESP32
-----------     -----
VCC        →    3.3V
DATA       →    GPIO 21
GND        →    GND
```

![[assets/IMG_3441.JPEG]]

If you connect to another pin make sure to set it in main.cpp
## Software Setup 💻

### Prerequisites

- [PlatformIO](https://platformio.org/) installed in VS Code
- Access to an MQTT broker (like EMQX, Mosquitto, etc.)

### Installation Steps

1. **Clone the repository:**
   ```bash
   git clone <your-repo-url>
   cd esp32-temp-controller
   ```

2. **Create secrets.h file:**
   
   Create a file called `secrets.h` in the `include/` directory with your credentials:
   
   ```cpp
   // include/secrets.h
   #ifndef SECRETS_H
   #define SECRETS_H
   
   // WiFi credentials
   #define WIFI_SSID "YourWiFiNetworkName"
   #define WIFI_PASSWORD "YourWiFiPassword"
   
   // MQTT broker settings
   #define MQTT_ADRESS "your.mqtt.broker.com"  // IP or hostname
   #define MQTT_PORT 1883                      // Standard MQTT port
   #define MQTT_USERNAME "your_username"
   #define MQTT_PASSWORD "your_password"
   
   #endif
   ```

3. **Install dependencies:**
   
   The `platformio.ini` should include:
   ```ini
   [env:esp32dev]
   platform = espressif32
   board = esp32dev
   framework = arduino
   lib_deps = 
       adafruit/DHT sensor library@^1.4.4
       knolleary/PubSubClient@^2.8
   ```

4. **Upload to ESP32:**
   ```bash
   pio run --target upload
   ```

## Configuration ⚙️

### Adjusting Sleep Time

Modify the sleep interval by changing the `SLEEP_TIME` constant in `main.cpp`:

```cpp
#define SLEEP_TIME 60  // Sleep time in seconds (default: 60s)
```

### Changing MQTT Topic

Update the MQTT topic by modifying:

```cpp
const char* mqtt_topic = "home";  // Base topic: home/sensors/dht22
```

### DHT22 Pin Configuration

Change the sensor pin by updating:

```cpp
#define DHT22_PIN 21  // ESP32 GPIO pin
```

## MQTT Data Format 📋

The device publishes data to: `{mqtt_topic}/sensors/dht22`

**Example payload:**
```json
{
  "temperature": 23.50,
  "humidity": 65.20,
  "device": "ESP32-DHT22"
}
```

## Power Consumption 🔋

- **Active time:** ~3-5 seconds per cycle
- **Deep sleep:** ~10-50 µA
- **Estimated battery life:** Several months on a single 18650 battery

## Testing MQTT Connection 🧪

Use mosquitto client tools to verify data reception:

```bash
# Subscribe to the topic
mosquitto_sub -h your.mqtt.broker.com -p 1883 -u username -P password -t "home/sensors/dht22"
```

## Troubleshooting 🔍

### Common Issues

1. **WiFi Connection Failed:**
   - Check SSID and password in `secrets.h`
   - Ensure ESP32 is in range of WiFi network

2. **MQTT Connection Failed:**
   - Verify broker address and credentials
   - Check if broker is accessible from your network
   - Ensure port 1883 is not blocked

3. **Sensor Reading NaN:**
   - Check DHT22 wiring connections
   - Verify sensor power supply (3.3V)
   - DHT22 needs 2-second delay between readings

4. **Deep Sleep Not Working:**
   - Ensure proper ESP32 board selection
   - Check if wake stub is properly configured

## Contributing 🤝

This is a personal fun project, but feel free to:
- Report issues
- Suggest improvements
- Submit pull requests

## License 📄

This project is open source and available under the [MIT License](LICENSE).

---

**Happy Monitoring!** 📊🎉