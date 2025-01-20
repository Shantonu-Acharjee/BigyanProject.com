#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

const char* ssid = "Sarah";
const char* password = "abcd1234efgh";

const int Pin1 = 3;
const int Pin2 = 2;
String Value = "data";
//Your Domain name with URL path or IP address with path
String serverName = "http://45.127.244.4/test.php?value=";

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;

void setup() {
  Serial.begin(115200); 
  pinMode(Pin1,INPUT);
  digitalWrite(Pin1,HIGH); 
  pinMode(Pin2,INPUT);
  digitalWrite(Pin2,HIGH);
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
}

void loop() {
  
  //Send an HTTP POST request every 10 minutes
  if ((millis() - lastTime) > timerDelay) {
    Serial.println("Read");
      boolean Value1 = digitalRead(Pin1);
      boolean Value2 = digitalRead(Pin2);
      if(Value1 == LOW){
        if(Value2 == LOW){
          Value = "data usege";
        }
        else{
          Value = "data wastage";
        }
        //Check WiFi connection status
        if(WiFi.status()== WL_CONNECTED){
          HTTPClient http;
          String serverPath = serverName + Value;
          
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
      }
    lastTime = millis();
  }
}
