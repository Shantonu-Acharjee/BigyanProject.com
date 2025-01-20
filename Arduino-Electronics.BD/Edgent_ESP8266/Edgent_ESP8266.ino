
// Fill-in information from your Blynk Template here
#define BLYNK_TEMPLATE_ID "TMPLWaGk-OLo"
#define BLYNK_DEVICE_NAME "Smart Office"

#define BLYNK_FIRMWARE_VERSION        "0.1.2"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

#define APP_DEBUG

// Uncomment your board, or configure a custom board in Settings.h
//#define USE_SPARKFUN_BLYNK_BOARD
#define USE_NODE_MCU_BOARD
//#define USE_WITTY_CLOUD_BOARD

#include "BlynkEdgent.h"

String content = "";
String HUM = "";
String TEM = "";
String SMO = "";
String CAR = "";
String RAI = "";
String MOT = "";
char character;

int ind1; // , locations
int ind2;
int ind3;
int ind4;
int ind5;
int ind6;

BLYNK_WRITE(V0)
{
  if(param.asInt()==1){
    digitalWrite(16, HIGH);
  }
  else{
    digitalWrite(16, LOW);
  }
}
BLYNK_WRITE(V1)
{
  if(param.asInt()==1){
    digitalWrite(13, HIGH);
  }
  else{
    digitalWrite(13, LOW);
  }
}
BLYNK_WRITE(V2)
{
  if(param.asInt()==1){
    digitalWrite(4, HIGH);
  }
  else{
    digitalWrite(4, LOW);
  }
}
BLYNK_WRITE(V3)
{
  if(param.asInt()==1){
    digitalWrite(5, HIGH);
  }
  else{
    digitalWrite(5, LOW);
  }
}

BLYNK_CONNECTED()
{
  Blynk.syncVirtual(V0);  

  Blynk.syncVirtual(V1);  

  Blynk.syncVirtual(V2);  

  Blynk.syncVirtual(V3);  
}

void setup()
{
  // Debug console
  Serial.begin(115200);
  pinMode(16, OUTPUT); // Initialise digital pin 2 as an output pin
  pinMode(13, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  BlynkEdgent.begin();
  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  BlynkEdgent.run();
  timer.run();
}
void sendSensor()
{
  if (Serial.available())  {
    char c = Serial.read();  //gets one byte from serial buffer
    if (c == '\n') {            
      Serial.flush();
      Serial.println(content);
      ind1 = content.indexOf('=');  //finds location of first ,
      HUM = content.substring(0, ind1);   //captures first data String
      ind2 = content.indexOf('=', ind1+1 );   //finds location of second ,
      TEM = content.substring(ind1+1, ind2);   //captures second data String
      ind3 = content.indexOf('=', ind2+1 );
      SMO = content.substring(ind2+1, ind3);
      ind4 = content.indexOf('=', ind3+1 );
      CAR = content.substring(ind3+1, ind4);
      ind5 = content.indexOf('=', ind4+1 );
      RAI = content.substring(ind4+1, ind5);
      ind6 = content.indexOf('=', ind5+1 );
      MOT = content.substring(ind6+1);
      content="";
      
      Blynk.virtualWrite(V4, HUM);
      Blynk.virtualWrite(V5, TEM);
      Blynk.virtualWrite(V6, SMO);
      Blynk.virtualWrite(V7, CAR);
      if(RAI == "1"){
        Blynk.virtualWrite(V8, "Rain Stop");
      }
      else{
        Blynk.virtualWrite(V8, "Rain Start");
      }
      if(MOT == "1"){
        Blynk.virtualWrite(V9, "Motion Detected");
      }
      else{
        Blynk.virtualWrite(V9, "No Motion");
      }
    }
    else{
      content += c;
    }
  }
}
