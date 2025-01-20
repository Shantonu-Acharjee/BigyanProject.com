#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <SimpleTimer.h>
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <ESP32Servo.h>
#include "DHT.h"
#define DHTPIN 23

#define DHTTYPE DHT11

const uint16_t kRecvPin = 36;

IRrecv irrecv(kRecvPin);

decode_results results;

DHT dht(DHTPIN, DHTTYPE);

char auth[] = "FG-0_i4LPv0N4lQ6r6nlFM_xTftVrlb3";

//WiFi credentials.
char ssid[] = "mobilewifi";
char pass[] = "12345678";

const int btnPin0 = 39;             // pin for physical push button switch.
const int btnPin1 = 34;             // pin for physical push button switch.
const int btnPin2 = 35;             // pin for physical push button switch.
const int btnPin3 = 32;             // pin for physical push button switch.
const int btnPin4 = 33;             // pin for physical push button switch.
const int btnPin5 = 25;             // pin for physical push button switch.
const int btnPin6 = 26;             // pin for physical push button switch.
const int btnPin7 = 27;             // pin for physical push button switch.
const int btnPin8 = 14;             // pin for physical push button switch.
const int RelayPin1 = 15;         // pin for relay output.
const int RelayPin2 = 2;         // pin for relay output.
const int RelayPin3 = 4;         // pin for relay output.
const int RelayPin4 = 5;         // pin for relay output.
const int RelayPin5 = 18;         // pin for relay output.
const int RelayPin6 = 19;         // pin for relay output.
const int RelayPin7 = 21;         // pin for relay output.
const int RelayPin8 = 3;         // pin for relay output.

ESP32Servo myservo;
int pos = 0;
int flag = 0;


BlynkTimer timer;
void checkPhysicalButton();
int btnPinState0 = LOW;           // ON
int btnPinState1 = LOW;           // ON
int btnPinState2 = LOW;           // ON
int btnPinState3 = LOW;           // ON
int btnPinState4 = LOW;           // ON
int btnPinState5 = LOW;           // ON
int btnPinState6 = LOW;           // ON
int btnPinState7 = LOW;           // ON
int btnPinState8 = LOW;           // ON
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
  irrecv.enableIRIn();
  Blynk.begin(auth, ssid, pass);
  pinMode(RelayPin1, OUTPUT);            //  initialize your pin as output.
  pinMode(RelayPin2, OUTPUT);            //  initialize your pin as output.
  pinMode(RelayPin3, OUTPUT);            //  initialize your pin as output.
  pinMode(RelayPin4, OUTPUT);            //  initialize your pin as output.
  pinMode(RelayPin5, OUTPUT);            //  initialize your pin as output.
  pinMode(RelayPin6, OUTPUT);            //  initialize your pin as output.
  pinMode(RelayPin7, OUTPUT);            //  initialize your pin as output.
  pinMode(RelayPin8, OUTPUT);            //  initialize your pin as output.
  pinMode(btnPin0, INPUT_PULLUP);        //  initialize your pin as input with enable internal pull-up resistor "input_pullup" 
  pinMode(btnPin1, INPUT_PULLUP);        //  initialize your pin as input with enable internal pull-up resistor "input_pullup"  
  pinMode(btnPin2, INPUT_PULLUP);        //  initialize your pin as input with enable internal pull-up resistor "input_pullup"  
  pinMode(btnPin3, INPUT_PULLUP);        //  initialize your pin as input with enable internal pull-up resistor "input_pullup"
  pinMode(btnPin4, INPUT_PULLUP);        //  initialize your pin as input with enable internal pull-up resistor "input_pullup"
  pinMode(btnPin5, INPUT_PULLUP);        //  initialize your pin as input with enable internal pull-up resistor "input_pullup"
  pinMode(btnPin6, INPUT_PULLUP);        //  initialize your pin as input with enable internal pull-up resistor "input_pullup"
  pinMode(btnPin7, INPUT_PULLUP);        //  initialize your pin as input with enable internal pull-up resistor "input_pullup"
  pinMode(btnPin8, INPUT_PULLUP);        //  initialize your pin as input with enable internal pull-up resistor "input_pullup"
  digitalWrite(RelayPin1, RelayPinState1);
  digitalWrite(RelayPin2, RelayPinState2);
  digitalWrite(RelayPin3, RelayPinState3);
  digitalWrite(RelayPin4, RelayPinState4);
  digitalWrite(RelayPin5, RelayPinState5);
  digitalWrite(RelayPin6, RelayPinState6);
  digitalWrite(RelayPin7, RelayPinState7);
  digitalWrite(RelayPin8, RelayPinState8);
  dht.begin();
  myservo.attach(22);

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
  if (digitalRead(btnPin0) == LOW)
  {
    if (btnPinState0 != LOW)
    {
      RelayPinState0 = !RelayPinState0;
      Blynk.virtualWrite(V0, RelayPinState0);
      if(RelayPinState0 == LOW){
        Serial.println("33");
        for(int posDegrees = 0; posDegrees <= 120; posDegrees++) {
          myservo.write(posDegrees);
          delay(20);
        }
      }
      else{
        Serial.println("44");
        for(int posDegrees = 120; posDegrees >= 0; posDegrees--) {
          myservo.write(posDegrees);
          delay(20);
        }
      }
    }

    btnPinState0 = LOW; 
  }
  else
  {
    btnPinState0 = HIGH;
  }
  if (digitalRead(btnPin1) == LOW)
  {
    if (btnPinState1 != LOW)
    {
      RelayPinState1 = !RelayPinState1;
      digitalWrite(RelayPin1, RelayPinState1);
      Blynk.virtualWrite(V1, RelayPinState1);
    }

    btnPinState1 = LOW; 
  }
  else
  {
    btnPinState1 = HIGH;
  }
  if (digitalRead(btnPin2) == LOW)
  {
    if (btnPinState2 != LOW)
    {
      RelayPinState2 = !RelayPinState2;
      digitalWrite(RelayPin2, RelayPinState2);
      Blynk.virtualWrite(V2, RelayPinState2);
    }

    btnPinState2 = LOW; 
  }
  else
  {
    btnPinState2 = HIGH;
  }
  if (digitalRead(btnPin3) == LOW)
  {
    if (btnPinState3 != LOW)
    {
      RelayPinState3 = !RelayPinState3;
      digitalWrite(RelayPin3, RelayPinState3);
      Blynk.virtualWrite(V3, RelayPinState3);
    }

    btnPinState3 = LOW; 
  }
  else
  {
    btnPinState3 = HIGH;
  }
  if (digitalRead(btnPin4) == LOW)
  {
    if (btnPinState4 != LOW)
    {
      RelayPinState4 = !RelayPinState4;
      digitalWrite(RelayPin4, RelayPinState4);
      Blynk.virtualWrite(V4, RelayPinState4);
    }

    btnPinState4 = LOW; 
  }
  else
  {
    btnPinState4 = HIGH;
  }
  if (digitalRead(btnPin5) == LOW)
  {
    if (btnPinState5 != LOW)
    {
      RelayPinState5 = !RelayPinState5;
      digitalWrite(RelayPin5, RelayPinState5);
      Blynk.virtualWrite(V5, RelayPinState5);
    }

    btnPinState5 = LOW; 
  }
  else
  {
    btnPinState5 = HIGH;
  }
  if (digitalRead(btnPin6) == LOW)
  {
    if (btnPinState6 != LOW)
    {
      RelayPinState6 = !RelayPinState6;
      digitalWrite(RelayPin6, RelayPinState6);
      Blynk.virtualWrite(V6, RelayPinState6);
    }

    btnPinState6 = LOW; 
  }
  else
  {
    btnPinState6 = HIGH;
  }
  if (digitalRead(btnPin7) == LOW)
  {
    if (btnPinState7 != LOW)
    {
      RelayPinState7 = !RelayPinState7;
      digitalWrite(RelayPin7, RelayPinState7);
      Blynk.virtualWrite(V7, RelayPinState7);
    }

    btnPinState7 = LOW; 
  }
  else
  {
    btnPinState7 = HIGH;
  }
  if (digitalRead(btnPin8) == LOW)
  {
    if (btnPinState8 != LOW)
    {
      RelayPinState8 = !RelayPinState8;
      digitalWrite(RelayPin8, RelayPinState8);
      Blynk.virtualWrite(V8, RelayPinState8);
    }

    btnPinState8 = LOW; 
  }
  else
  {
    btnPinState8 = HIGH;
  }
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  String stringOne, stringTwo, stringThree, stringFour, stringFive;
  stringOne = String("T= ");
  stringTwo = String(" C         H= ");
  stringThree = String(" %");
  stringFour = String();
  stringFour = stringOne + t + stringTwo + h + stringThree;
  Blynk.virtualWrite(V9, stringFour);
}

void loop()
{
  Blynk.run();                               // Run Blynk
  timer.run();                               // Run SimpleTimer
  if(irrecv.decode(&results)){
    // print() & println() can't handle printing long longs. (uint64_t)
    serialPrintUint64(results.value);
    Serial.println("");
    irrecv.resume();  // Receive the next value
    if(results.value == 16724175){
      Serial.println("1");
      RelayPinState1 = !RelayPinState1;
      digitalWrite(RelayPin1, RelayPinState1);
      Blynk.virtualWrite(V1, RelayPinState1);
      delay(100);
    }
    else if(results.value == 16718055){
      Serial.println("2");
      RelayPinState2 = !RelayPinState2;
      digitalWrite(RelayPin2, RelayPinState2);
      Blynk.virtualWrite(V2, RelayPinState2);
      delay(100);
    }
    else if(results.value == 16743045){
      Serial.println("3");
      RelayPinState3 = !RelayPinState3;
      digitalWrite(RelayPin3, RelayPinState3);
      Blynk.virtualWrite(V3, RelayPinState3);
      delay(100);
    }
    else if(results.value == 16716015){
      Serial.println("4");
      RelayPinState4 = !RelayPinState4;
      digitalWrite(RelayPin4, RelayPinState4);
      Blynk.virtualWrite(V4, RelayPinState4);
      delay(100);
    }
    else if(results.value == 16726215){
      Serial.println("5");
      RelayPinState5 = !RelayPinState5;
      digitalWrite(RelayPin5, RelayPinState5);
      Blynk.virtualWrite(V5, RelayPinState5);
      delay(100);
    }
    else if(results.value == 16734885){
      Serial.println("6");
      RelayPinState6 = !RelayPinState6;
      digitalWrite(RelayPin6, RelayPinState6);
      Blynk.virtualWrite(V6, RelayPinState6);
      delay(100);
    }
    else if(results.value == 16728765){
      Serial.println("7");
      RelayPinState7 = !RelayPinState7;
      digitalWrite(RelayPin7, RelayPinState7);
      Blynk.virtualWrite(V7, RelayPinState7);
      delay(100);
    }
    else if(results.value == 16730805){
      Serial.println("8");
      RelayPinState8 = !RelayPinState8;
      digitalWrite(RelayPin8, RelayPinState8);
      Blynk.virtualWrite(V8, RelayPinState8);
      delay(100);
    }
    else if(results.value == 16753245){
      Serial.println("off");
      RelayPinState0 = !RelayPinState0;
      Blynk.virtualWrite(V0, RelayPinState0);
      if(RelayPinState0 == LOW){
        Serial.println("33");
        for(int posDegrees = 0; posDegrees <= 120; posDegrees++) {
          myservo.write(posDegrees);
          delay(20);
        }
      }
      else{
        Serial.println("44");
        for(int posDegrees = 120; posDegrees >= 0; posDegrees--) {
          myservo.write(posDegrees);
          delay(20);
        }
      }
      delay(100);
    }
    else{
      delay(500);
    }
  }
  
}
