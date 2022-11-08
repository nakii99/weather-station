#include <DHT.h>
#include "Anemometer.hpp"
#define DHTPIN 2
#define DHTTYPE DHT11 
#define ANEMOPIN 3

DHT dht(DHTPIN, DHTTYPE);
Anemometer anemo(ANEMOPIN);

void setup() {
  Serial.begin(115200);
  dht.begin();
  anemo.begin();
  Serial.println("WS#INIT");
}


void loop() {
  delay(2000);

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C"));
  Serial.print(F(" Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C\n"));

  anemo();
}
