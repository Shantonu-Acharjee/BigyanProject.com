#include <SoftwareSerial.h> 
#include <ESP8266WiFi.h>

String apiKey = "U9BUCYTL0EHY2ARC"; // Enter your Write API key from ThingSpeak
const char *ssid = "Electronics.Com.BD";     // replace with your wifi ssid and wpa2 key
const char *pass = "abcd1234efgh";
const char* server = "api.thingspeak.com";
String content = "";
String ph = "";
String color = "";
String spe = "";
String temp = "";
String pressure = "";
String flow = "";
String volume = "";
char character;

int ind1; // , locations
int ind2;
int ind3;
int ind4;
int ind5;
int ind6;
int ind7;

WiFiClient client;

void setup()
{
  // Debug console
  Serial.begin(115200);
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
  if (Serial.available())  {
    char c = Serial.read();  //gets one byte from serial buffer
    if (c == '\n') {
      
  Serial.flush();
  ind1 = content.indexOf('=');  //finds location of first ,
  ph = content.substring(0, ind1);   //captures first data String
  ind2 = content.indexOf('=', ind1+1 );   //finds location of second ,
  color = content.substring(ind1+1, ind2);   //captures second data String
  ind3 = content.indexOf('=', ind2+1 );
  spe = content.substring(ind2+1, ind3);
  ind4 = content.indexOf('=', ind3+1 );
  temp = content.substring(ind3+1,ind4);
  ind5 = content.indexOf('=', ind4+1 );
  pressure = content.substring(ind4+1,ind5);
  ind6 = content.indexOf('=', ind5+1 );
  flow = content.substring(ind5+1,ind6);
  ind7 = content.indexOf('=', ind6+1 );
  volume = content.substring(ind6+1,ind7); //captures remain part of data after last ,

  Serial.print("ph = ");
  Serial.println(ph); 
  Serial.print("color = ");
  Serial.println(color);
  Serial.print("spe = ");
  Serial.println(spe);
  Serial.print("temp = ");
  Serial.println(temp);
  Serial.print("pressure = ");
  Serial.println(pressure);
  Serial.print("flow = ");
  Serial.println(flow);
  Serial.print("volume = ");
  Serial.println(volume);
  Serial.println();
  content="";
  if (client.connect(server, 80)) // "184.106.153.149" or api.thingspeak.com
  {
    String postStr = apiKey;
      postStr += "&field1=";
      postStr += String(temp);
      postStr += "&field2=";
      postStr += String(ph);
      postStr += "&field3=";
      postStr += String(color);
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
    else{
      content += c;
    }
  Serial.println("---------------------xxxxx--------------------");
  }
}
