
// Fill-in information from your Blynk Template here
#define BLYNK_TEMPLATE_ID "TMPL6vQsGybs"
#define BLYNK_DEVICE_NAME "Curtain"

#define BLYNK_FIRMWARE_VERSION        "0.1.1"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

#define APP_DEBUG
#define USE_NODE_MCU_BOARD

#include "BlynkEdgent.h"

const int DIR = 12;
const int STEP = 14;
const int EndStopL = 5;
const int EndStopR = 4;
const int OpenPin = 10;
const int ClosePin = 9;
const int StopPin = 13;

boolean stopflag = false;
boolean V0pinStatus = false;
boolean V1pinStatus = false;
boolean OpenStatus = false;
boolean CloseStatus = false;

ICACHE_RAM_ATTR void Interrupted() {
  Serial.println("Interrupt!!!");
  OpenStatus = false;
  CloseStatus = false;
}
BLYNK_WRITE(V0)
{
  int pinValue = param.asInt();
  if(pinValue == 1){
    OpenStatus = true;
  }
  else{
    OpenStatus = false;
  }
}
BLYNK_WRITE(V1)
{
  int pinValue = param.asInt();
  if(pinValue == 1){
    CloseStatus = true;
  }
  else{
    CloseStatus = false;
  }
}
BLYNK_CONNECTED()
{
  // Request the latest state from the server
  Blynk.syncVirtual(V0);
  Blynk.syncVirtual(V1);
}
void setup()
{
  Serial.begin(115200);
  delay(100);
  pinMode(STEP, OUTPUT);
  pinMode(DIR, OUTPUT);
  pinMode(EndStopL, INPUT_PULLUP);
  pinMode(EndStopR, INPUT_PULLUP);
  pinMode(OpenPin, INPUT_PULLUP);
  pinMode(ClosePin, INPUT_PULLUP);
  pinMode(StopPin, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(StopPin), Interrupted, RISING);

  BlynkEdgent.begin();
  Blynk.virtualWrite(V0, 0);
  Blynk.virtualWrite(V1, 0);
}

void loop() {
  BlynkEdgent.run();
  /*if(digitalRead(OpenPin) == LOW){
    OpenStatus = true;
  }
  if(digitalRead(ClosePin) == LOW){
    CloseStatus = true;
  }
  if(OpenStatus == true){
    CloseStatus = false;
    OpenCurtain();
  }
  if(CloseStatus == true){
    OpenStatus = false;
    CloseCurtain();
  }*/
}

void OpenCurtain(){
  if(digitalRead(EndStopL) == LOW){
    if(V0pinStatus == false){
      Blynk.virtualWrite(V0, 1);
      V0pinStatus = true;
    }
    digitalWrite(DIR, HIGH);
    digitalWrite(STEP, HIGH);
    delayMicroseconds(500);
    digitalWrite(STEP, LOW);
    delayMicroseconds(500);
  }
  else{
    if(V0pinStatus == true){
      Blynk.virtualWrite(V0, 0);
      V0pinStatus = false;
    }  
  }
}

void CloseCurtain(){
  if(digitalRead(EndStopR) == LOW){
    if(V1pinStatus == false){
      Blynk.virtualWrite(V1, 1);
      V1pinStatus = true;
    }
    digitalWrite(DIR, LOW);
    digitalWrite(STEP, HIGH);
    delayMicroseconds(500);
    digitalWrite(STEP, LOW);
    delayMicroseconds(500);
  } 
  else{
    if(V1pinStatus == true){
      Blynk.virtualWrite(V1, 0);
      V1pinStatus = false;
    }   
  }
}
