#define BLYNK_PRINT Serial
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#define LDR 14
#define PUMP 13
#define M_Sensor A0
#define R_Sensor 12
BlynkTimer timer;

// Set these to run example.
#define FIREBASE_HOST "irrigation-4f042-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "4r65vizSPYoJwzWUImJLDLsIFTYUhZxA0EtzSIaW"
#define WIFI_SSID "Panda" //provide ssid (wifi name)
#define WIFI_PASSWORD "crowmsk1998" //wifi password

#define BLYNK_AUTH "vYdIwxa7mgLtgICQLMD2MvQ2U7o39BFr"


int LDR_value = 0;
int M_Sensor_value = 0;
int R_Sensor_value = 0;
int flag = 0;
void setup()
{
  Serial.begin(115200);
  pinMode(PUMP, OUTPUT);
  pinMode(LDR, INPUT);
  pinMode(R_Sensor, INPUT);
   WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
   Serial.print("connecting");
   while (WiFi.status() != WL_CONNECTED)
   {
   Serial.print(".");
   delay(500);
   }
   Serial.println();
 Serial.print("connected: ");
 Serial.println(WiFi.localIP());
 Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
if(Firebase.failed())
 {
 Serial.print(Firebase.error());
 }
 else{
 
 Serial.print("Firebase Connected");
 }
  Blynk.begin(BLYNK_AUTH, WIFI_SSID, WIFI_PASSWORD);
  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  Blynk.run();
  timer.run();
}

BLYNK_CONNECTED()
{
  // Request the latest state from the server
  Blynk.syncVirtual(V0);
  Blynk.syncVirtual(V4);
}
BLYNK_WRITE(V0)
{                           
  int RelayPinState = param.asInt();
  digitalWrite(PUMP, RelayPinState);
  if(RelayPinState == LOW){
    Firebase.setString("PUMP/Value","ON");
  }
  else{
    Firebase.setString("PUMP/Value","OFF");
  }
}
BLYNK_WRITE(V4)
{                           
  switch (param.asInt()){
   case 1: {
    flag = 0;
    break;
   }
   case 2: {
    flag = 1;
    break;
   }
  }
}
void sendSensor()
{    
  M_Sensor_value = analogRead(M_Sensor); 
  Serial.println(M_Sensor_value);
  LDR_value = digitalRead(LDR);
  Serial.println(LDR_value);
  R_Sensor_value = digitalRead(R_Sensor);
  Serial.println(R_Sensor_value);
  if(LDR_value == LOW){
    Blynk.virtualWrite(V1,"Sunny");
    Firebase.setString("Sensors/LDR/Value","Sunny");
  }
  else{
    Blynk.virtualWrite(V1,"Cloudy");
    Firebase.setString("Sensors/LDR/Value","Cloudy");
  }
  if(R_Sensor_value == LOW){
    Blynk.virtualWrite(V2,"Raining");
    Firebase.setString("Sensors/Rain/Value","Raining");
    if(flag == 0){
      Blynk.virtualWrite(V0,HIGH);
      digitalWrite(PUMP, HIGH);
      Firebase.setString("PUMP/Value","OFF");
    }
  }
  else{ 
    Blynk.virtualWrite(V2,"Not Raining");
    Firebase.setString("Sensors/Rain/Value","Not Raining");
  }
  Blynk.virtualWrite(V3,M_Sensor_value);
  Firebase.setInt("Sensors/Moisture/Value",M_Sensor_value);
  if(M_Sensor_value > 400){
    if(flag == 0){
      Blynk.virtualWrite(V0,HIGH);
      digitalWrite(PUMP, HIGH);
      Firebase.setString("PUMP/Value","OFF");
    }
  }  
  if(M_Sensor_value < 5){
    if(flag == 0){
      Blynk.virtualWrite(V0,LOW);
      digitalWrite(PUMP, LOW);
      Firebase.setString("PUMP/Value","ON");
    }
  }
}
