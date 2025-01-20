#include <SoftwareSerial.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F,16,2);
#include "DHT.h"
#define DHTPIN 7
#define Sensor A0
#define DHTTYPE DHT11
int flag = 0;
int barzzer = 6;
DHT dht(DHTPIN, DHTTYPE);
SoftwareSerial mySerial(11, 12);
void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(barzzer, OUTPUT);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Initializing......");
  delay(15000);
  dht.begin();
  
}

void loop() {
  
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  //Serial.print(F("Humidity: "));
  //Serial.print(h);
  //Serial.print(F("%  Temperature: "));
  //Serial.print(t);
  //Serial.println(analogRead(A0));
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(t);
  lcd.setCursor(8, 0);
  lcd.print("H:");
  lcd.print(h);
  if(analogRead(A0) > 670){
    lcd.setCursor(0, 1);
    lcd.print("Water Level High");
  }
  else if(analogRead(A0) > 660){
    lcd.setCursor(0, 1);
    lcd.print("Water Level Mid");
  }
  else{
    lcd.setCursor(0, 1);
    lcd.print("Water Level Low");
    digitalWrite(barzzer, HIGH);
    if(flag == 0){ 
    send_sms1(); 
    flag = 1; 
    }
  }
  if(t>40){
    digitalWrite(barzzer, HIGH);
    send_sms();
  }
  delay(1000);
  digitalWrite(barzzer,LOW);
}

void send_sms()
{  
  mySerial.println("AT");
  updateSerial();

  mySerial.println("AT+CMGF=1");
  updateSerial();
  mySerial.println("AT+CMGS=\"+8801648901533\"");
  updateSerial();
  mySerial.print("Alert High Temperature.!");
  updateSerial();
  mySerial.write(26);
}
void send_sms1()
{  
  mySerial.println("AT");
  updateSerial();

  mySerial.println("AT+CMGF=1");
  updateSerial();
  mySerial.println("AT+CMGS=\"+8801648901533\"");
  updateSerial();
  mySerial.print("Alert Water Level Low!");
  updateSerial();
  mySerial.write(26);
}
void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());
  }
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());
  }
}
