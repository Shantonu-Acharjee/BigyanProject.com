#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
SoftwareSerial uno(2,3); // (Rx, Tx)

//String apiKey = "1YHES877CLY8VYZZ"; // Enter your Write API key from ThingSpeak
String apiKey = "YW1H2AJV459AS557"; // Enter your Write API key from ThingSpeak
const char *ssid = "Sarah";     // replace with your wifi ssid and wpa2 key
const char *pass = "abcd1234efgh";
const char* server = "api.thingspeak.com";
String content = "";
String bmp = "";
String oxi = "";
String cmo = "";
String tem = "";
String hum = "";
char character;

int ind1; // , locations
int ind2;
int ind3;
int ind4;
int ind5;

WiFiClient client;

void setup()
{
  // Debug console
  Serial.begin(115200);
  uno.begin(115200);
  pinMode(3,INPUT);
  pinMode(2,OUTPUT);
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
      //do stuff
      
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
  tem = content.substring(ind3+1,ind4);
  ind5 = content.indexOf('=', ind4+1 );
  hum = content.substring(ind4+1); //captures remain part of data after last ,

  Serial.print("bmp = ");
  Serial.println(bmp); 
  Serial.print("oxi = ");
  Serial.println(oxi);
  Serial.print("cmo = ");
  Serial.println(cmo);
  Serial.print("tem = ");
  Serial.println(tem);
  Serial.print("hum = ");
  Serial.println(hum);
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
      postStr += "&field5=";
      postStr += String(hum);
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
  //Serial.println("---------------------xxxxx--------------------");
  /*
    */
  }
}
