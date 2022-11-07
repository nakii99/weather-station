#include <DHT.h>
#define DHTPIN 2
#define DHTTYPE DHT11 

DHT dht(DHTPIN, DHTTYPE);


#define ANEMOPIN 3

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 1000; 
int Count = 0;

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

  Serial.println("WS#INIT");
}


void anemo() {
  if ((millis() - lastDebounceTime) > debounceDelay)
  {
    lastDebounceTime = millis();
    Serial.print((Count * 8.75)/100);
    Count = 0;
    Serial.println("m/s");
  }
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
