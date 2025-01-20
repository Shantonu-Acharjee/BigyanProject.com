#define BLYNK_PRINT Serial

#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <ArduinoJson.h>
SoftwareSerial s(D7,D6);
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "OIbiI7EAIIfdf-NY4TKKPjDyd4EeClXt";
//char auth[] = "1aTwbXWt-Yywh5NYsKvZj5yV-KS2W0p5";
char* input;
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "MiPhone";
char pass[] = "1234567890";
void setup()
{
  // Debug console
  Serial.begin(9600);
  s.begin(9600);
  //while (!Serial) continue;
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  Blynk.run();
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(s);
  if (root == JsonObject::invalid())
    return;
 
  Serial.println("JSON received and parsed");
  //root.prettyPrintTo(Serial);
  Serial.print("voltage ");
  float voltage=root["voltage"];
  Serial.print(voltage);
  Serial.print("   current ");
  float current=root["current"];
  Serial.print(current);
  Serial.println("");
  Serial.println("---------------------xxxxx--------------------");
  Blynk.virtualWrite(V0,voltage);
  Blynk.virtualWrite(V1,current);
  Blynk.virtualWrite(V2,voltage*current);
}
