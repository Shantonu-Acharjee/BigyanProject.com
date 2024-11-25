#include <WiFi.h> // Include the WiFi library

const char* ssid = "shantonu";     // Replace with your Wi-Fi SSID
const char* password = "12345678"; // Replace with your Wi-Fi password

void setup() {
  Serial.begin(115200); // Initialize serial communication
  Serial.println("Connecting to Wi-Fi...");

  WiFi.begin(ssid, password); // Connect to Wi-Fi

  while (WiFi.status() != WL_CONNECTED) { // Wait until the ESP32 is connected to Wi-Fi
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Wi-Fi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP()); // Print the assigned IP address
}

void loop() {
  // The ESP32 is connected to Wi-Fi
}
