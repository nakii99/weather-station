#include "Arduino.h"
#include "Anemometer.hpp"

uint8_t anemoPin = 0;
unsigned long lastDebounceTime = 0; 
unsigned long debounceDelay = 1000; 
int Count = 0; 

void initAnemometer(uint8_t pin) {
    anemoPin = pin;
    pinMode(anemoPin, INPUT_PULLUP);
    attachInterrupt( digitalPinToInterrupt(anemoPin), onChange, FALLING);
}

void onChange() {
  if ( digitalRead(anemoPin) == LOW )
    Count++;
}

float readWindSpeed() {
  if ((millis() - lastDebounceTime) > debounceDelay)
  {
    lastDebounceTime = millis();
    float currentSpeed = (Count * 8.75)/100;
    Count = 0;
    return currentSpeed;
  }
  return 0;
}
