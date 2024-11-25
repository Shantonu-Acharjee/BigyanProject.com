#include <WiFi.h> // Include the WiFi library

const char* ssid = "ESP32_Hotspot";     // Define the Wi-Fi hotspot name (SSID)
const char* password = "12345678";      // Define the password (minimum 8 characters)

void setup() {
  Serial.begin(115200); // Start serial communication
  Serial.println("Setting up Wi-Fi Hotspot...");

  // Configure the ESP32 as an Access Point
  WiFi.softAP(ssid, password);

  // Print the IP address of the hotspot
  IPAddress IP = WiFi.softAPIP();
  Serial.println("Hotspot Created!");
  Serial.print("Hotspot IP Address: ");
  Serial.println(IP);
}

void loop() {
  // You can add logic here if needed (e.g., serving clients)
}
