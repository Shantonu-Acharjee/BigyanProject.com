#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <DFRobot_HX711.h>
#include "DHT.h"
#define DHTPIN 3
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
DFRobot_HX711 MyScale(5, 4);
#define REPORTING_PERIOD_MS     1000
 
PulseOximeter pox;

uint32_t tsLastReport = 0;
 
void onBeatDetected()
{
    Serial.println("Beat!");
}
void setup() {
    Serial.begin(115200);
    Serial.print("Initializing pulse oximeter..");
    if (!pox.begin()) {
        Serial.println("FAILED");
        for(;;);
    } else {
        Serial.println("SUCCESS");
    }
    dht.begin();
     pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
 
    // Register a callback for the beat detection
    pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop() {
  pox.update();
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    float HeartRate = pox.getHeartRate();
    int SpO2 = pox.getSpO2();
      Serial.print("Heart rate:");
      Serial.print(pox.getHeartRate());
      Serial.print("bpm / SpO2:");
      Serial.print(pox.getSpO2());
      Serial.print("%");   
      float t = dht.readTemperature();
      Serial.print(F(" RTemp:"));
      Serial.print(t);
      Serial.print(F("°C ")); 
      Serial.print(MyScale.readWeight(), 1);
      Serial.println(" g");  
      tsLastReport = millis();
  }
}
