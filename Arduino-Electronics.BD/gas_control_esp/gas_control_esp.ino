#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Wire.h>
#include "i2c-scanner.h"
#include "crc16.h"
const char* ssid = "Sarah";
const char* password = "abcd1234efgh";

String data = "";
//Your Domain name with URL path or IP address with path
String serverName = "http://103.83.207.35/soft/test.php?value=";

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;

void setup() {
  Serial.begin(115200); 
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
  Wire.begin(0, 2);
}

void loop() {
  int flag = 0;
  Wire.beginTransmission(9); /* begin with device address 8 */
  Wire.write("check");  /* sends hello string */
  Wire.endTransmission();    /* stop transmitting */
  Wire.requestFrom(8, 13); /* request & read data of size 13 from slave */
  while(Wire.available()){
    data += (char)Wire.read();
  }
  if(data == "usage"){
    Serial.println(data);
    flag = 1;
  }
  else if(data == "wastage"){
    Serial.println(data);
    flag = 1;
  }
  else{
    Serial.println(data);
    flag = 0;
  }
  //if(flag == 1)
  //{
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;
      String serverPath = serverName + data;
      
      // Your Domain name with URL path or IP address with path
      http.begin(serverPath.c_str());
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
 // }
  Serial.println();
  delay(5000);
}
