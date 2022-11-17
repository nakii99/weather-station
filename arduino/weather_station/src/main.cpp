#include <Arduino.h>
#include <DHT.h>
#include <Adafruit_BMP280.h>
#include "Anemometer.hpp"

#define DHTPIN 2
#define DHTTYPE DHT11 
#define ANEMOPIN 3

DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP280 bmp;

String inputString = "";      
bool stringComplete = false;  


void setup() {
  Serial.begin(115200);
  dht.begin();
  initAnemometer(ANEMOPIN);

  if(!bmp.begin(BMP280_ADDRESS_ALT)) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                      "try a different address!"));
  }
  bmp.setSampling();

  inputString.reserve(200);
  Serial.println(F("WS#INIT"));
}

void loop() {
  if (stringComplete) {
    Serial.println(inputString);
    inputString.trim();  
    if(inputString == "TEST") {
      Serial.println("secret");
    } 
    



    inputString = "";
    stringComplete = false;
  }

  delay(1000);

  float t = dht.readTemperature();
  float h = dht.readHumidity();
  float speed = readWindSpeed();

   Serial.print(F("Temperature = "));
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");

    Serial.print(F("Pressure = "));
    Serial.print(bmp.readPressure());
    Serial.println(" Pa");

    Serial.print(F("Approx altitude = "));
    Serial.print(bmp.readAltitude(1013.25)); /* Adjusted to local forecast! */
    Serial.println(" m");

  Serial.print(F("WS_HUM#"));
  Serial.println(h);

  Serial.print(F("WS_TEMP#"));
  Serial.println(t);

  Serial.print(F("WS_ANEMO#"));
  Serial.println(speed);
}


void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\r') {
      stringComplete = true;
    }
  }
}
