#include "Anemometer.hpp"


class Anemometer {
    Anemometer(int pin) {
        *this->pin = pin;
    }

    void begin() {
         pinMode(pin, INPUT_PULLUP);
        attachInterrupt( digitalPinToInterrupt(pin), onChange, FALLING);
    }

    void onChange() {
        if ( digitalRead(pin) == LOW )
        Count++;
    }

    float getValue() {
    if ((millis() - lastDebounceTime) > debounceDelay)
    {
        lastDebounceTime = millis();
        float toReturn = (Count * 8.75)/100;
        Count = 0;
        return toReturn;
    }
    return 0;
}

}





