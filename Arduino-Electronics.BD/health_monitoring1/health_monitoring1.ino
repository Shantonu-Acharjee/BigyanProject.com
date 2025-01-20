#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>
#include "MAX30100_PulseOximeter.h"
#include "DHT.h"
#define DHTTYPE DHT22
#define DHTPIN 5
#define REPORTING_PERIOD_MS     1000
SoftwareSerial nodeMcu(4,3); // (Rx, Tx)
LiquidCrystal_I2C lcd(0x27,20,4);
PulseOximeter pox;
uint32_t tsLastReport = 0;
String str;
DHT dht(DHTPIN, DHTTYPE);
void onBeatDetected()
{
    Serial.println("Beat!");
}
 
void setup()
{
    Serial.begin(115200);
    nodeMcu.begin(115200);
    lcd.init();
    lcd.backlight();
    Serial.print("Initializing pulse oximeter..");
    if (!pox.begin()) {
        Serial.println("FAILED");
        for(;;);
    } else {
        Serial.println("SUCCESS");
    }
    pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
    pox.setOnBeatDetectedCallback(onBeatDetected);
    dht.begin();
}
 
void loop()
{
    // Make sure to call update as fast as possible
    pox.update();
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {      
        lcd.clear();
        Serial.print("Heart rate:");
        float bmp = pox.getHeartRate();
        Serial.print(bmp);
        Serial.print("bpm / SpO2:");
        float oxi = pox.getSpO2();
        Serial.print(oxi);
        Serial.print("%");
        int cmo = analogRead(A0);
        Serial.print(" cmo:  ");
        Serial.print(cmo);
        Serial.print(" : ");
        Serial.println(oxi);
        float h = dht.readHumidity();
        float t = dht.readTemperature();        
        lcd.setCursor(0,0);
        lcd.print("BMP:");
        lcd.print(int(bmp));
        lcd.print(" OXI:");
        lcd.print(oxi);
        lcd.print("%");
        lcd.setCursor(0,1);
        lcd.print("TEM:");
        lcd.print(int(t));
        lcd.print(" HUM:");
        lcd.print(int(h));
        lcd.print("%");
        str =String(bmp)+String("=")+String(oxi)+String("=")+String(cmo)+String("=")+String(t)+String("=")+String(h);
        nodeMcu.println(str);
        Serial.println("Json Sent");
        tsLastReport = millis();
    }
}
