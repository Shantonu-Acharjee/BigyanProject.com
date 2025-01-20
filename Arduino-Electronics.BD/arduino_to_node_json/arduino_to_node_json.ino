#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
SoftwareSerial uno(D1,D2); // (Rx, Tx)

char ssid[] = "Sarah";
char pass[] = "abcd1234efgh";
void setup()
{
  // Debug console
  Serial.begin(9600);
  uno.begin(9600);
  pinMode(D1,INPUT);
  pinMode(D2,OUTPUT);
}

void loop()
{
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(uno);
  if (root == JsonObject::invalid())
    return;
 
  Serial.println("JSON received and parsed");
  //root.prettyPrintTo(Serial);
  Serial.print("Status ");
  String Status=root["Data1"];
  Serial.print(Status);
  Serial.print("   RFID ");
  String RFID=root["Data2"];
  Serial.print(RFID);
  Serial.println("");
  Serial.println("---------------------xxxxx--------------------");
}
