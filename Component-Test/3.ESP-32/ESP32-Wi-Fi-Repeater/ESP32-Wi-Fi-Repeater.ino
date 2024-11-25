#include <WiFi.h> // Include the WiFi library

// Replace these with your Wi-Fi network credentials
const char* ssid = "shantonu";       // Wi-Fi network SSID (existing network)
const char* password = "12345678";   // Wi-Fi network password

// Access Point credentials
const char* ap_ssid = "ESP32_Repeater";  // Repeater SSID
const char* ap_password = "12345678";    // Repeater password (minimum 8 characters)

void setup() {
  Serial.begin(115200); // Start serial communication
  Serial.println("Starting Wi-Fi Repeater...");

  // Connect to the existing Wi-Fi network
  WiFi.mode(WIFI_AP_STA); // Enable both Station and Access Point mode
  WiFi.begin(ssid, password);

  Serial.print("Connecting to Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected to existing Wi-Fi network!");

  // Print the IP address of the ESP32
  Serial.print("Station IP Address: ");
  Serial.println(WiFi.localIP());

  // Start the Access Point
  WiFi.softAP(ap_ssid, ap_password);
  IPAddress AP_IP = WiFi.softAPIP();
  Serial.println("Repeater Access Point Started!");
  Serial.print("AP IP Address: ");
  Serial.println(AP_IP);
}

void loop() {
  // Optional: Add logic to handle packets or debug
}
