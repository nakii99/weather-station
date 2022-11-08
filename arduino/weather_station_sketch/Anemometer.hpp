#pragma once

class Anemometer {
    public:
        Anemometer(int pin);
        void begin();
        void onChange();
        float getValue();

    private:
        unsigned long lastDebounceTime = 0; 
        unsigned long debounceDelay = 1000; 
        int Count = 0;
        int pin;    
}