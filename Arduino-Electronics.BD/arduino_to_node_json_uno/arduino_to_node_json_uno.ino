#include <SoftwareSerial.h>
#include <ArduinoJson.h>
SoftwareSerial nodeMcu(3,2); // (Rx, Tx)


void setup()
{
  // Debug console
  Serial.begin(9600);
  nodeMcu.begin(9600);
}

void loop()
{
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["Data1"] = "Shawon";
  root["Data2"] = "007";
 // if(uno.available()>0)
  //{
   root.printTo(nodeMcu);
   Serial.println("Json Sent");
 // }
 // else{
 //  Serial.println("Uno Not Found");    
 // }
  
  delay(1000);
}
