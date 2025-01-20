#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT22
LiquidCrystal_I2C lcd(0x27,16,2);
DHT dht(DHTPIN, DHTTYPE);

const int Light = 3;
const int Humidifier = 4;
const int Fan = 5;
const int Tem_red_Led = 6;
const int Tem_green_Led = 7;
const int Hum_red_Led = 8;
const int Hum_green_Led = 9;

void setup()
{
  Serial.begin(9600);
  pinMode(Light,OUTPUT);
  pinMode(Humidifier,OUTPUT);
  pinMode(Fan,OUTPUT);
  pinMode(Tem_red_Led,OUTPUT);
  pinMode(Tem_green_Led,OUTPUT);
  pinMode(Hum_red_Led,OUTPUT);
  pinMode(Hum_green_Led,OUTPUT);
  digitalWrite(Light,HIGH);
  digitalWrite(Humidifier,HIGH);
  digitalWrite(Fan,LOW);
  digitalWrite(Tem_red_Led,LOW);
  digitalWrite(Tem_green_Led,LOW);
  digitalWrite(Hum_red_Led,LOW);
  digitalWrite(Hum_green_Led,LOW);
  lcd.init();
  lcd.backlight();
  dht.begin();
}

void loop()
{
  delay(2000);
  lcd.clear();
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int g = analogRead(A0);
  //Serial.print(F("Humidity: "));
  //Serial.print(h);
  //Serial.print(F("%  Temperature: "));
  //Serial.print(t);
  //Serial.println(F("°C "));
  //Serial.print(F("Gas: "));
  //Serial.print(g);
  //Serial.println(F("ppm"));
  lcd.setCursor(0,0);
  lcd.print("Tem  ");
  lcd.print("  | ");
  lcd.print("Hum");
  lcd.setCursor(0,1);
  lcd.print(t);
  lcd.print("C");
  lcd.setCursor(7,1);
  lcd.print("|");
  lcd.setCursor(9,1);
  lcd.print(h);
  lcd.print("%");
  if(t<31)
  {
    digitalWrite(Light,LOW);
    digitalWrite(Fan,LOW);
    digitalWrite(Tem_red_Led,LOW);
    digitalWrite(Tem_green_Led,HIGH);
  }
  else if(t>40){
    digitalWrite(Light,HIGH);
    digitalWrite(Fan,HIGH);
    digitalWrite(Tem_red_Led,HIGH);
    digitalWrite(Tem_green_Led,LOW);
  }
  else{
    digitalWrite(Light,HIGH);
    digitalWrite(Fan,LOW);
    digitalWrite(Tem_red_Led,LOW);
    digitalWrite(Tem_green_Led,LOW);
  }
  if(h<44)
  {
    digitalWrite(Humidifier,LOW);
    digitalWrite(Hum_red_Led,LOW);
    digitalWrite(Hum_green_Led,HIGH);
  }
  else if(h>55){
    digitalWrite(Humidifier,HIGH);
    digitalWrite(Hum_red_Led,HIGH);
    digitalWrite(Hum_green_Led,LOW);
  }
  else{
    digitalWrite(Humidifier,HIGH);
    digitalWrite(Hum_red_Led,LOW);
    digitalWrite(Hum_green_Led,LOW);
  }
  String str =String(h)+String("=")+String(t)+String("=")+String(g);
  Serial.println(str);
}
