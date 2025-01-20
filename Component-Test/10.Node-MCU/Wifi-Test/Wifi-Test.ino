//Generic ESP8266 Module

#include <ESP8266WiFi.h>

const char* ssid = "111111";      // Replace with your Wi-Fi SSID
const char* password = "12345678";  // Replace with your Wi-Fi password

void setup() {
  Serial.begin(115200);  // Initialize Serial Monitor
  WiFi.begin(ssid, password);  // Connect to Wi-Fi

  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nConnected to Wi-Fi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());  // Print ESP8266 IP address
}

void loop() {
  // Your code here
}
