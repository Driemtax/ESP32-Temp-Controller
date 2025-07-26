#include <Arduino.h>
#include <DHT.h>
#include <WiFi.h>
#include <secrets.h>

#define DHT22_PIN 21 // ESP32 pin GPIO21 connected to DHT22 sensor

// set your wifi name and password here
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD; 

// put function declarations here:
DHT dht22(DHT22_PIN, DHT22);

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
  

  Serial.println("Initializing DHT22...");
  dht22.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  // read humidity and temp
  float hum = dht22.readHumidity();
  float tempC = dht22.readTemperature();

  // Check wether reading was successful
  if (isnan(tempC) | isnan(hum)) {
    Serial.println("Failed to read from DHT22 sensor!");
  } else {
    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.print("%");

    Serial.print("  |  ");

    Serial.print("Temperature: ");
    Serial.print(tempC);
    Serial.print("°C");

    Serial.print("\n");
  }

  // wait 2 seconds between readings
  delay(2000);
}

// put function definitions here: