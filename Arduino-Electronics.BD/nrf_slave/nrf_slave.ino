#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";
const int BuzzerPower = 13;
const int Buzzer = 10;
void setup() {
  pinMode(BuzzerPower, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  digitalWrite(BuzzerPower,LOW);
  digitalWrite(Buzzer,LOW);
  lcd.init(); 
  lcd.backlight();
  Serial.begin(9600);
  lcd.clear();  
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}
void loop() {
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.println(text);
    
  }
}
