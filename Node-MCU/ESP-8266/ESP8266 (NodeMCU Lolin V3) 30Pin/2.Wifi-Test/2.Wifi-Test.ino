/*
********* ESP8266 (NodeMCU Lolin V3) 30Pin ************

1.Install "CH341SER.EXE" Driver
2.File > Preference > Add This URL "http://arduino.esp8266.com/stable/package_esp8266com_index.json" to Additional Board Manager URLs.
3.Tools > Board > Board Manager > Install "esp8266" by ESP8266 Comunity
4.Tools > Board > esp8266 > NodeMCU 1.0 (ESP-12E Module).
*/

#include <ESP8266WiFi.h>

const char* ssid = "shantonu";      // Replace with your Wi-Fi SSID
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
