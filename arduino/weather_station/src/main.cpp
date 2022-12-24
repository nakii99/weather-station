#include <Arduino.h>
#include <DHT.h>
#include <Adafruit_BMP280.h>
#include <SD.h>
#include <RTClib.h>
#include "Anemometer.hpp"
#include "ReadSensors.hpp"

#define DHTPIN 5
#define DHTTYPE DHT11 
#define ANEMOPIN 2

DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP280 bmp;
File sdFile;
RTC_DS1307 rtc;
DateTime currentDateTime;

String inputString = "";      
bool stringComplete = false;  

String getSensorString(float hum, float temp, float speed, float pressure, float altitude) {
  String toReturn;
  toReturn.concat(hum);
  toReturn.concat(",");
  toReturn.concat(temp);
  toReturn.concat(",");
  toReturn.concat(speed);
  toReturn.concat(",");
  toReturn.concat(pressure);
  toReturn.concat(",");
  toReturn.concat(altitude);
  toReturn.concat(",");

  return toReturn;
}

String readSensors() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  float speed = readWindSpeed();
  float pressure = bmp.readPressure();
  float altitude = bmp.readAltitude(1013.25);

  String test = getSensorString(hum,temp,speed,pressure,altitude);
  Serial.println(test);

  return test;
}

void readString() {
   if (stringComplete) {
    Serial.println(inputString);
    inputString.trim();  
   
    if(inputString == "READ_SD") {
      File toSend = SD.open("values.csv", FILE_READ);
      String myStr;
      if(toSend) {
        while(toSend.available()) {
          Serial.write(toSend.read());
        }
        toSend.close();
      }
    } 
   
    inputString = "";
    stringComplete = false;
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  initAnemometer(ANEMOPIN);
  bmp.begin(BMP280_ADDRESS_ALT);
  bmp.setSampling();
  

  if(!SD.begin(10)) {
    Serial.println("Error initializing SD card");
    while(1);
  }

  if(!rtc.begin()) {
    Serial.println("Error initializing real time clock");
    while(1);
  }

  if(!rtc.isrunning()) {
    //Time adjustment when rtc is not running
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));  
  }

  if(!SD.exists("values.csv")) {
    sdFile = SD.open("values.csv",FILE_WRITE);
    sdFile.println("Humidity,Temperature,Speed,Pressure,Altitude,DateTime");
    sdFile.close();
  }

  inputString.reserve(200);
}

void loop() {
  String toBeWritten;

  readString();

  toBeWritten = readSensors();

  currentDateTime = rtc.now();
  toBeWritten.concat(currentDateTime.timestamp());

  sdFile = SD.open("values.csv",FILE_WRITE);
  sdFile.println(toBeWritten);
  sdFile.close();


  delay(5000); 
}


void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == '\r') {
      stringComplete = true;
    }
  }
}
