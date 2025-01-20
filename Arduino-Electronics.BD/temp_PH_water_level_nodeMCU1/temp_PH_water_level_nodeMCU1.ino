#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
SoftwareSerial uno(D1,D2); // (Rx, Tx)

String apiKey = "7C1B9QQ5J3Y1CFDS"; // Enter your Write API key from ThingSpeak
const char *ssid = "Sarah";     // replace with your wifi ssid and wpa2 key
const char *pass = "abcd1234efgh";
const char* server = "api.thingspeak.com";

WiFiClient client;

void setup()
{
  // Debug console
  Serial.begin(9600);
  uno.begin(9600);
  pinMode(D1,INPUT);
  pinMode(D2,OUTPUT);
  Serial.println("Connecting to ");
  Serial.println(ssid);
  
  
  WiFi.begin(ssid, pass);
  
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

void loop()
{
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(uno);
  if (root == JsonObject::invalid())
    return;
 
  Serial.println("JSON received and parsed");
  //root.prettyPrintTo(Serial);
  Serial.print("Temp ");
  String Temp=root["Temp"];
  Serial.print(Temp);
  Serial.print(" PH ");
  String PH=root["PH"];
  Serial.print(PH);
  Serial.print("   Level ");
  String Level=root["Level"];
  Serial.print(Level);
  Serial.println("");
  Serial.println("---------------------xxxxx--------------------");
  if (client.connect(server, 80)) // "184.106.153.149" or api.thingspeak.com
  {
    String postStr = apiKey;
      postStr += "&field1=";
      postStr += String(Temp);
      postStr += "&field2=";
      postStr += String(PH);
      postStr += "&field3=";
      postStr += String(Level);
      postStr += "\r\n\r\n";
    
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
   
  }
    client.stop();
}
