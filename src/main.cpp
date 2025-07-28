#include <Arduino.h>
#include <DHT.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <secrets.h>

#define DHT22_PIN 21 // ESP32 pin GPIO21 connected to DHT22 sensor
#define uS_TO_S_FACTOR 1000000ULL // needed for esp sleep mode
#define SLEEP_TIME 60

// set your wifi name and password here
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD; 
const char* mqtt_adress = MQTT_ADRESS;
const char* mqtt_username = MQTT_USERNAME;
const char* mqtt_password = MQTT_PASSWORD;
const char* mqtt_topic = "home";
const int mqtt_port = MQTT_PORT;

// MQTT and WiFi clients
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

// put function declarations here:
DHT dht22(DHT22_PIN, DHT22);

void connectToMQTT() {
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP32Client-" + String(random(0xffff), HEX);
    
    if (mqttClient.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void publishSensorData(float temperature, float humidity) {
  if (mqttClient.connected()) {
    // Create JSON-like payload
    String payload = "{\"temperature\":" + String(temperature, 2) + 
                    ",\"humidity\":" + String(humidity, 2) + 
                    ",\"device\":\"ESP32-DHT22\"}";
    
    String topic = String(mqtt_topic) + "/sensors/dht22";
    mqttClient.publish(topic.c_str(), payload.c_str());
    
    Serial.println("Data published to MQTT:");
    Serial.println("Topic: " + topic);
    Serial.println("Payload: " + payload);
  }
  delay(200);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(1000);

  Serial.println("Setting up a wifi connection...");
  WiFi.mode(WIFI_STA); // Station mode to connect to router
  WiFi.begin(ssid, password);

  short connectionTryCounter = 0;
  while (WiFi.status() != WL_CONNECTED) {
    if (connectionTryCounter >= 100)
    {
      Serial.println("WiFi connection failed.");
      break;
    }
    Serial.print(".");
    delay(100);
    connectionTryCounter++;
  }

  if (connectionTryCounter < 100) {
    Serial.println("\nConnected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());
  }
  
  // Initialize MQTT
  Serial.println("Setting up MQTT connection...");
  mqttClient.setServer(mqtt_adress, mqtt_port);
  connectToMQTT();

  if (mqttClient.connected())
  Serial.println("Initializing DHT22...");
  {
    dht22.begin();
    float temp = dht22.readTemperature();
    float hum = dht22.readHumidity();

    // format and send data
    publishSensorData(temp, hum);
  }
  mqttClient.disconnect();
  WiFi.disconnect(true);
  delay(50);

  // Timer for 60s deep sleep
  esp_sleep_enable_timer_wakeup(SLEEP_TIME * uS_TO_S_FACTOR);
  esp_deep_sleep_start();
}

void loop() {
  // Maintain MQTT connection
  if (!mqttClient.connected()) {
    connectToMQTT();
  }
  mqttClient.loop();

    // Serial.print("Humidity: ");
    // Serial.print(hum);
    // Serial.print("%");

    // Serial.print("  |  ");

    // Serial.print("Temperature: ");
    // Serial.print(tempC);
    // Serial.print("°C");

    // Serial.print("\n");
}

// put function definitions here: