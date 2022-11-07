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


float anemo() {
  if ((millis() - lastDebounceTime) > debounceDelay)
  {
    lastDebounceTime = millis();
    float currentSpeed = (Count * 8.75)/100;
    Count = 0;
    return currentSpeed;
  }
  return 0;
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
  float speed = anemo();

  
  Serial.print("WS_HUM#");
  Serial.println(h);

  Serial.print("WS_TEMP#");
  Serial.println(t);

  Serial.print("WS_ANEMO#");
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
