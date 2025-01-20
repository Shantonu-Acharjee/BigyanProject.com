#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL6l85qfYiH"
#define BLYNK_TEMPLATE_NAME "Smart Home"
#define BLYNK_AUTH_TOKEN "vvbj5gcGcHCSVDQTSmSVd4Qu6oh7skg6"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>
BlynkTimer timer; 
Servo myservo; 
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Project";
char pass[] = "12345678";

const int RelayPin0 = D2;         // pin for relay output.
const int RelayPin1 = D5;         // pin for relay output.
const int RelayPin2 = D6;          // pin for relay output.
const int RelayPin3 = D7;         // pin for relay output.
int sensorPin = A0;
int sensorValue = 0;
int pos = 0;
int flag = 0;
int flag1 = 0;
int count = 0;
int RelayPinState0 = HIGH;        // OFF
int RelayPinState1 = HIGH;        // OFF
int RelayPinState2 = HIGH;        // OFF
int RelayPinState3 = HIGH;        // OFF

BLYNK_CONNECTED()
{
  Blynk.syncVirtual(V0);
  Blynk.syncVirtual(V1);
  Blynk.syncVirtual(V2);
  Blynk.syncVirtual(V3);
  Blynk.syncVirtual(V4);
  Blynk.syncVirtual(V5);

}
BLYNK_WRITE(V0)
{   
  RelayPinState0 = param.asInt();
  digitalWrite(RelayPin0, RelayPinState0);  
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
BLYNK_WRITE(V5)
{                           
  if(param.asInt() == HIGH && flag == 0){
    for(int posDegrees = 0; posDegrees <= 150; posDegrees++) {
      myservo.write(posDegrees);
      delay(20);
    }
    flag = 1;
  }
  else if(param.asInt() == LOW && flag == 1){
    for(int posDegrees = 150; posDegrees >= 0; posDegrees--) {
      myservo.write(posDegrees);
      delay(20);
    }
    flag = 0;
  }
}



void setup()
{
  Serial.begin(9600);
  
  pinMode(RelayPin0, OUTPUT);            //  initialize your pin as output.
  pinMode(RelayPin1, OUTPUT);            //  initialize your pin as output.
  pinMode(RelayPin2, OUTPUT);            //  initialize your pin as output.
  pinMode(RelayPin3, OUTPUT);            //  initialize your pin as output.
  digitalWrite(RelayPin0, RelayPinState0);
  digitalWrite(RelayPin1, RelayPinState1);
  digitalWrite(RelayPin2, RelayPinState2);
  digitalWrite(RelayPin3, RelayPinState3);
  myservo.attach(D1);  // attaches the servo on GIO2 to the servo object
  myservo.write(0);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, myTimer); 
}



void loop()
{
  Blynk.run();                               // Run Blynk
  // runs BlynkTimer
  timer.run();
}
void myTimer() 
{
  sensorValue = analogRead(sensorPin);
  sensorValue = map(sensorValue, 0, 1024, 0, 100);
  Blynk.virtualWrite(V4, sensorValue);
  Serial.println(sensorValue);
  if(sensorValue > 45 && flag1 == 0)
  {
    Serial.println("gas_alert");
    Blynk.logEvent("gas_alert","GAS detected...! take action immediately.");
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
