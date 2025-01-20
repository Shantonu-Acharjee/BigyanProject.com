/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example runs directly on NodeMCU.

  Note: This requires ESP8266 support package:
    https://github.com/esp8266/Arduino

  Please be sure to select the right NodeMCU module
  in the Tools -> Board menu!

  For advanced settings please follow ESP examples :
   - ESP8266_Standalone_Manual_IP.ino
   - ESP8266_Standalone_SmartConfig.ino
   - ESP8266_Standalone_SSL.ino

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
char auth[] = "cjsk_wqX6BS6AP1jsZcagKp-sqD7JWBX";

char ssid[] = "Electronics.Com.BD";
char pass[] = "abcd1234efgh";
int latchPin = D7;  //--> Pin connected to ST_CP of 74HC595
int clockPin = D5;  //--> Pin connected to SH_CP of 74HC595
int dataPin = D6;   //--> Pin connected to DS of 74HC595

byte SR4LEDS = 0;  //--> The byte variable to be entered into the Second 
byte SR3LEDS = 0;  //--> The byte variable to be entered into the Second 
byte SR2LEDS = 0;  //--> The byte variable to be entered into the Second 74HC595
byte SR1LEDS = 0;  //--> The byte variable to be entered into the First 74HC595

BLYNK_CONNECTED()
{
  Blynk.syncVirtual(V0);
  Blynk.syncVirtual(V1);
  Blynk.syncVirtual(V2);
  Blynk.syncVirtual(V3);
  Blynk.syncVirtual(V4);
  Blynk.syncVirtual(V5);
  Blynk.syncVirtual(V6);
  Blynk.syncVirtual(V7);
  Blynk.syncVirtual(V8);
  Blynk.syncVirtual(V9);
  Blynk.syncVirtual(V10);
  Blynk.syncVirtual(V11);
  Blynk.syncVirtual(V12);
  Blynk.syncVirtual(V13);
  Blynk.syncVirtual(V14);
  Blynk.syncVirtual(V15);
  Blynk.syncVirtual(V16);
  Blynk.syncVirtual(V17);
  Blynk.syncVirtual(V18);
  Blynk.syncVirtual(V19);
  Blynk.syncVirtual(V20);
  Blynk.syncVirtual(V21);
  Blynk.syncVirtual(V22);
  Blynk.syncVirtual(V23);
  Blynk.syncVirtual(V24);
  Blynk.syncVirtual(V25);
  Blynk.syncVirtual(V26);
  Blynk.syncVirtual(V27);
}
BLYNK_WRITE(V0)
{   
  if(param.asInt() == HIGH){
    SR1LEDS = B10000000;
    SR2LEDS = B00000000;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  }
  else{
    SR1LEDS = B00000000;
    SR2LEDS = B00000000;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
  }
}
BLYNK_WRITE(V1)
{   
  if(param.asInt() == HIGH){
    SR1LEDS = B01000000;
    SR2LEDS = B00000000;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  }
  else{
    SR1LEDS = B00000000;
    SR2LEDS = B00000000;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
  }
}
BLYNK_WRITE(V2)
{   
  if(param.asInt() == HIGH){
    SR1LEDS = B00100000;
    SR2LEDS = B00000000;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  }
  else{
    SR1LEDS = B00000000;
    SR2LEDS = B00000000;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
  }
}
BLYNK_WRITE(V3)
{   
  if(param.asInt() == HIGH){
    SR1LEDS = B00010000;
    SR2LEDS = B00000000;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  }
  else{
    SR1LEDS = B00000000;
    SR2LEDS = B00000000;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
  }
}
BLYNK_WRITE(V4)
{   
  if(param.asInt() == HIGH){
    SR1LEDS = B00001000;
    SR2LEDS = B00000000;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  }
  else{
    SR1LEDS = B00000000;
    SR2LEDS = B00000000;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
  }
}
BLYNK_WRITE(V5)
{   
  if(param.asInt() == HIGH){
    SR1LEDS = B00000100;
    SR2LEDS = B00000000;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  }
  else{
    SR1LEDS = B00000000;
    SR2LEDS = B00000000;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
  }
}
BLYNK_WRITE(V6)
{   
  if(param.asInt() == HIGH){
    SR1LEDS = B00000010;
    SR2LEDS = B00000000;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  }
  else{
    SR1LEDS = B00000000;
    SR2LEDS = B00000000;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
  }
}
BLYNK_WRITE(V7)
{   
  if(param.asInt() == HIGH){
    SR1LEDS = B00000001;
    SR2LEDS = B00000000;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  }
  else{
    SR1LEDS = B00000000;
    SR2LEDS = B00000000;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
  }
}
BLYNK_WRITE(V8)
{   
  if(param.asInt() == HIGH){
    SR1LEDS = B00000000;
    SR2LEDS = B10000000;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  }
  else{
    SR1LEDS = B00000000;
    SR2LEDS = B00000000;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
  }
}
BLYNK_WRITE(V9)
{   
  if(param.asInt() == HIGH){
    SR1LEDS = B00000000;
    SR2LEDS = B01000000;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  }
  else{
    SR1LEDS = B00000000;
    SR2LEDS = B00000000;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
  }
}
BLYNK_WRITE(V10)
{   
  if(param.asInt() == HIGH){
    SR1LEDS = B00000000;
    SR2LEDS = B00100000;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  }
  else{
    SR1LEDS = B00000000;
    SR2LEDS = B00000000;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
  }
}
BLYNK_WRITE(V11)
{   
  if(param.asInt() == HIGH){
    SR1LEDS = B00000000;
    SR2LEDS = B00010000;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  }
  else{
    SR1LEDS = B00000000;
    SR2LEDS = B00000000;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
  }
}
BLYNK_WRITE(V12)
{   
  if(param.asInt() == HIGH){
    SR1LEDS = B00000000;
    SR2LEDS = B00001000;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  }
  else{
    SR1LEDS = B00000000;
    SR2LEDS = B00000000;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
  }
}
BLYNK_WRITE(V13)
{   
  if(param.asInt() == HIGH){
    SR1LEDS = B00000000;
    SR2LEDS = B00000100;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  }
  else{
    SR1LEDS = B00000000;
    SR2LEDS = B00000000;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
  }
}
BLYNK_WRITE(V14)
{   
  if(param.asInt() == HIGH){
    SR1LEDS = B00000000;
    SR2LEDS = B00000010;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  }
  else{
    SR1LEDS = B00000000;
    SR2LEDS = B00000000;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
  }
}
BLYNK_WRITE(V15)
{   
  if(param.asInt() == HIGH){
    SR1LEDS = B00000000;
    SR2LEDS = B00000001;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  }
  else{
    SR1LEDS = B00000000;
    SR2LEDS = B00000000;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
  }
}
BLYNK_WRITE(V16)
{   
  if(param.asInt() == HIGH){
    SR1LEDS = B00000000;
    SR2LEDS = B00000000;
    SR3LEDS = B10000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  }
  else{
    SR1LEDS = B00000000;
    SR2LEDS = B00000000;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
  }
}
BLYNK_WRITE(V17)
{   
  if(param.asInt() == HIGH){
    SR1LEDS = B00000000;
    SR2LEDS = B00000000;
    SR3LEDS = B01000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  }
  else{
    SR1LEDS = B00000000;
    SR2LEDS = B00000000;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
  }
}
BLYNK_WRITE(V18)
{   
  if(param.asInt() == HIGH){
    SR1LEDS = B00000000;
    SR2LEDS = B00000000;
    SR3LEDS = B00100000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  }
  else{
    SR1LEDS = B00000000;
    SR2LEDS = B00000000;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
  }
}
BLYNK_WRITE(V19)
{   
  if(param.asInt() == HIGH){
    SR1LEDS = B00000000;
    SR2LEDS = B00000000;
    SR3LEDS = B00010000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  }
  else{
    SR1LEDS = B00000000;
    SR2LEDS = B00000000;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
  }
}
BLYNK_WRITE(V20)
{   
  if(param.asInt() == HIGH){
    SR1LEDS = B00000000;
    SR2LEDS = B00000000;
    SR3LEDS = B00001000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  }
  else{
    SR1LEDS = B00000000;
    SR2LEDS = B00000000;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
  }
}
BLYNK_WRITE(V21)
{   
  if(param.asInt() == HIGH){
    SR1LEDS = B00000000;
    SR2LEDS = B00000000;
    SR3LEDS = B00000100;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  }
  else{
    SR1LEDS = B00000000;
    SR2LEDS = B00000000;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
  }
}
BLYNK_WRITE(V22)
{   
  if(param.asInt() == HIGH){
    SR1LEDS = B00000000;
    SR2LEDS = B00000000;
    SR3LEDS = B00000010;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  }
  else{
    SR1LEDS = B00000000;
    SR2LEDS = B00000000;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
  }
}
BLYNK_WRITE(V23)
{   
  if(param.asInt() == HIGH){
    SR1LEDS = B00000000;
    SR2LEDS = B00000000;
    SR3LEDS = B00000001;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  }
  else{
    SR1LEDS = B00000000;
    SR2LEDS = B00000000;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
  }
}
BLYNK_WRITE(V24)
{   
  if(param.asInt() == HIGH){
    SR1LEDS = B00000000;
    SR2LEDS = B00000000;
    SR3LEDS = B00000000;
    SR4LEDS = B10000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  }
  else{
    SR1LEDS = B00000000;
    SR2LEDS = B00000000;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
  }
}
BLYNK_WRITE(V25)
{   
  if(param.asInt() == HIGH){
    SR1LEDS = B00000000;
    SR2LEDS = B00000000;
    SR3LEDS = B00000000;
    SR4LEDS = B01000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  }
  else{
    SR1LEDS = B00000000;
    SR2LEDS = B00000000;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
  }
}
BLYNK_WRITE(V26)
{   
  if(param.asInt() == HIGH){
    SR1LEDS = B00000000;
    SR2LEDS = B00000000;
    SR3LEDS = B00000000;
    SR4LEDS = B00100000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  }
  else{
    SR1LEDS = B00000000;
    SR2LEDS = B00000000;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
  }
}
BLYNK_WRITE(V27)
{   
  if(param.asInt() == HIGH){
    SR1LEDS = B00000000;
    SR2LEDS = B00000000;
    SR3LEDS = B00000000;
    SR4LEDS = B00010000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  }
  else{
    SR1LEDS = B00000000;
    SR2LEDS = B00000000;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
  }
}


void setup()
{
  Serial.begin(9600);
  
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);

  Blynk.begin(auth, ssid, pass);
  updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
}

void loop()
{
  Blynk.run();
}
void updateShiftRegister(byte SR1, byte SR2, byte SR3, byte SR4)  {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, SR4);
  shiftOut(dataPin, clockPin, LSBFIRST, SR3);
  shiftOut(dataPin, clockPin, LSBFIRST, SR2);
  shiftOut(dataPin, clockPin, LSBFIRST, SR1);
  digitalWrite(latchPin, HIGH);
}
