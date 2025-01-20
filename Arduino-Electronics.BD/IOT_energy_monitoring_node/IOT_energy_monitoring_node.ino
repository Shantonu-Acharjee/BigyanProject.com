#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
SoftwareSerial uno(14,12); // (Rx, Tx)

String apiKey = "VA8TPFOPH5DXBFS0";
const char *ssid = "Redmi";     // replace with your wifi ssid and wpa2 key
const char *pass = "1234567890";
const char* server = "api.thingspeak.com";
String content = "";
String bmp = "";
String oxi = "";
String cmo = "";
String tem = "";
char character;

int ind1; // , locations
int ind2;
int ind3;
int ind4;

WiFiClient client;

void setup()
{
  // Debug console
  Serial.begin(115200);
  uno.begin(115200);
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
  if (uno.available())  {
    char c = uno.read();  //gets one byte from serial buffer
    if (c == '\n') {      
      Serial.println();
      Serial.print("captured String is : "); 
      Serial.println(content); //prints string to serial port out
      
      uno.flush();
      ind1 = content.indexOf('=');  //finds location of first ,
      bmp = content.substring(0, ind1);   //captures first data String
      ind2 = content.indexOf('=', ind1+1 );   //finds location of second ,
      oxi = content.substring(ind1+1, ind2);   //captures second data String
      ind3 = content.indexOf('=', ind2+1 );
      cmo = content.substring(ind2+1, ind3);
      ind4 = content.indexOf('=', ind3+1 );
      tem = content.substring(ind3+1); //captures remain part of data after last ,
    
      Serial.print("bmp = ");
      Serial.println(bmp); 
      Serial.print("oxi = ");
      Serial.println(oxi);
      Serial.print("cmo = ");
      Serial.println(cmo);
      Serial.print("tem = ");
      Serial.println(tem);
      Serial.println();
      content="";
      if (client.connect(server, 80)) // "184.106.153.149" or api.thingspeak.com
      {
        String postStr = apiKey;
          postStr += "&field1=";
          postStr += String(bmp);
          postStr += "&field2=";
          postStr += String(oxi);
          postStr += "&field3=";
          postStr += String(cmo);
          postStr += "&field4=";
          postStr += String(tem);
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
      else{
        Serial.println("Server Not Respond");
      }
      client.stop();
    }
    else{
      content += c;
    }
    Serial.println("---------------------xxxxx--------------------");
  }
}
