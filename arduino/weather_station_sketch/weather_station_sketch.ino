#include <DHT.h>
#define DHTPIN 2
#define DHTTYPE DHT11 

DHT dht(DHTPIN, DHTTYPE);


#define ANEMOPIN 3

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 1000; 
int Count = 0;

String inputString = "";      // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

void onChange()
{
  if ( digitalRead(ANEMOPIN) == LOW )
    Count++;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  dht.begin();

  pinMode(ANEMOPIN, INPUT_PULLUP);
  attachInterrupt( digitalPinToInterrupt(ANEMOPIN), onChange, FALLING);
  inputString.reserve(200);
  Serial.println("WS#INIT");
}


void anemo() {
  if ((millis() - lastDebounceTime) > debounceDelay)
  {
    lastDebounceTime = millis();
    float currentSpeed = (Count * 8.75)/100;
    Serial.print("WS_ANEMO#");
    Serial.println(currentSpeed);
    Count = 0;
  }
}


void loop() {
  if (stringComplete) {
    Serial.println(inputString);
    // clear the string:
    inputString = "";
    stringComplete = false;
  }

  delay(1000);

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print("WS_HUM#");
  Serial.println(h);

  Serial.print("WS_TEMP#");
  Serial.println(t);

  anemo();

  
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
