#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <SimpleTimer.h>
#include <Arduino.h>
#include <ESP32Servo.h>
#include "DHT.h"
#define DHTPIN 4
#define MQPIN 15

#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

char auth[] = "FG-0_i4LPv0N4lQ6r6nlFM_xTftVrlb3";

//WiFi credentials.
char ssid[] = "mobilewifi";
char pass[] = "12345678";

const int RelayPin1 = 16;         // pin for relay output.
const int RelayPin2 = 17;         // pin for relay output.
const int RelayPin3 = 5;         // pin for relay output.
const int RelayPin4 = 18;         // pin for relay output.
const int RelayPin5 = 19;         // pin for relay output.
const int RelayPin6 = 21;         // pin for relay output.
const int RelayPin7 = 22;         // pin for relay output.
const int RelayPin8 = 23;         // pin for relay output.

ESP32Servo myservo;
int pos = 0;
int flag = 0;
int flag1 = 0;
int count = 0;

BlynkTimer timer;
void checkPhysicalButton();
int RelayPinState0 = HIGH;        // OFF
int RelayPinState1 = HIGH;        // OFF
int RelayPinState2 = HIGH;        // OFF
int RelayPinState3 = HIGH;        // OFF
int RelayPinState4 = HIGH;        // OFF
int RelayPinState5 = HIGH;        // OFF
int RelayPinState6 = HIGH;        // OFF
int RelayPinState7 = HIGH;        // OFF
int RelayPinState8 = HIGH;        // OFF

void setup()
{
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  pinMode(RelayPin1, OUTPUT);            //  initialize your pin as output.
  pinMode(RelayPin2, OUTPUT);            //  initialize your pin as output.
  pinMode(RelayPin3, OUTPUT);            //  initialize your pin as output.
  pinMode(RelayPin4, OUTPUT);            //  initialize your pin as output.
  pinMode(RelayPin5, OUTPUT);            //  initialize your pin as output.
  pinMode(RelayPin6, OUTPUT);            //  initialize your pin as output.
  pinMode(RelayPin7, OUTPUT);            //  initialize your pin as output.
  pinMode(RelayPin8, OUTPUT);            //  initialize your pin as output.
  pinMode(MQPIN, INPUT);            //  initialize your pin as output.
  digitalWrite(RelayPin1, RelayPinState1);
  digitalWrite(RelayPin2, RelayPinState2);
  digitalWrite(RelayPin3, RelayPinState3);
  digitalWrite(RelayPin4, RelayPinState4);
  digitalWrite(RelayPin5, RelayPinState5);
  digitalWrite(RelayPin6, RelayPinState6);
  digitalWrite(RelayPin7, RelayPinState7);
  digitalWrite(RelayPin8, RelayPinState8);
  dht.begin();
  myservo.attach(2);

  // Setup a function to be called every 100 ms
  timer.setInterval(100L, checkPhysicalButton);
}

// Every time we connect to the cloud...
BLYNK_CONNECTED()
{
  // Request the latest state from the server
  Blynk.syncVirtual(V0);
  Blynk.syncVirtual(V1);
  Blynk.syncVirtual(V2);
  Blynk.syncVirtual(V3);
  Blynk.syncVirtual(V4);
  Blynk.syncVirtual(V5);
  Blynk.syncVirtual(V6);
  Blynk.syncVirtual(V7);
  Blynk.syncVirtual(V8);
}

// When App button is pushed - switch the state
// Map this Virtual Pin to your Mobile Blynk apps widget.
BLYNK_WRITE(V0)
{   
  RelayPinState0 = param.asInt();
  if(RelayPinState0 == LOW && flag == 0){
    Serial.println("11");
    for(int posDegrees = 0; posDegrees <= 120; posDegrees++) {
      myservo.write(posDegrees);
      delay(20);
    }
    flag = 1;
  }
  else if(RelayPinState0 == HIGH && flag == 1){
    Serial.println("22");
    for(int posDegrees = 120; posDegrees >= 0; posDegrees--) {
      myservo.write(posDegrees);
      delay(20);
    }
    flag = 0;
  }
  
}
BLYNK_WRITE(V1)
{                           
  RelayPinState1 = param.asInt();
  digitalWrite(RelayPin1, RelayPinState1);
}
BLYNK_WRITE(V2)
{                           
  RelayPinState2 = param.asInt();
  digitalWrite(RelayPin2, RelayPinState2);
}
BLYNK_WRITE(V3)
{                           
  RelayPinState3 = param.asInt();
  digitalWrite(RelayPin3, RelayPinState3);
}
BLYNK_WRITE(V4)
{                           
  RelayPinState4 = param.asInt();
  digitalWrite(RelayPin4, RelayPinState4);
}
BLYNK_WRITE(V5)
{                           
  RelayPinState5 = param.asInt();
  digitalWrite(RelayPin5, RelayPinState5);
}
BLYNK_WRITE(V6)
{                           
  RelayPinState6 = param.asInt();
  digitalWrite(RelayPin6, RelayPinState6);
}
BLYNK_WRITE(V7)
{                           
  RelayPinState7 = param.asInt();
  digitalWrite(RelayPin7, RelayPinState7);
}
BLYNK_WRITE(V8)
{                           
  RelayPinState8 = param.asInt();
  digitalWrite(RelayPin8, RelayPinState8);
}

void checkPhysicalButton()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  String stringOne, stringTwo, stringThree, stringFour, stringFive;
  stringOne = String("T= ");
  stringTwo = String(" C         H= ");
  stringThree = String(" %");
  stringFour = String();
  stringFour = stringOne + t + stringTwo + h + stringThree;
  Blynk.virtualWrite(V9, stringFour);
  if(!digitalRead(MQPIN) && flag1 == 0)
  {
    Blynk.notify("GAS detected...! take action immediately.");
    flag1=1;
    count=0;
  }
  else if(flag1==1){
    count++;
    if(count == 600)
    {
      flag1=0;
    }
  }
}

void loop()
{
  Blynk.run();                               // Run Blynk
  timer.run();                               // Run SimpleTimer  
}
