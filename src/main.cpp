#include <Arduino.h>
#include <DHT.h>

#define DHT22_PIN 21 // ESP32 pin GPIO21 connected to DHT22 sensor

// put function declarations here:
DHT dht22(DHT22_PIN, DHT22);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Begin init phase...");
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
