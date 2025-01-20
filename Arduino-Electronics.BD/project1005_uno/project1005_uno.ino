#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>

#define RedLed 4
#define GreenLed 5
#define Buzzer 6

SoftwareSerial nodeMcu(3,2); // (Rx, Tx)
LiquidCrystal_I2C lcd(0x27,16,2);

void setup() {
  Serial.begin(9600);
  nodeMcu.begin(9600);
  pinMode(Buzzer, OUTPUT);
  pinMode(RedLed, OUTPUT);
  pinMode(GreenLed, OUTPUT);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Initialising....");
  delay(2000);
}

void loop() {
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(nodeMcu);
  if (root == JsonObject::invalid())
    return;
  String Data1=root["Data1"];
  String Data2=root["Data2"];
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(Data1);
  lcd.setCursor(0,1);
  lcd.print(Data2);
  if(Data2 == "Access Granted"){ 
    digitalWrite(GreenLed,HIGH);
    for(int i=0; i<255; i++) { 
      analogWrite(Buzzer, i); 
      delay(10); 
    }     
    digitalWrite(GreenLed,LOW);
  }
  if(Data2 == "Access Denied"){ 
    digitalWrite(RedLed,HIGH);
    for(int i=255; i>0; i--) { 
      analogWrite(Buzzer, i); 
      delay(10);     
    }    
    digitalWrite(RedLed,LOW);
  }
  digitalWrite(Buzzer,LOW);
}
