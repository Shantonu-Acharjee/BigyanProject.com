#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Replace these with your WiFi network credentials
const char* ssid = "Junaed_Home";
const char* password = "helloworld";

ESP8266WebServer server(80);

void handleRoot() {
  String html = "<html><body>";
  html += "<h1>Motor and Bin Control</h1>";
  html += "<p><a href=\"/command?State=F\"><button>Forward</button></a></p>";
  html += "<p><a href=\"/command?State=B\"><button>Backward</button></a></p>";
  html += "<p><a href=\"/command?State=L\"><button>Left</button></a></p>";
  html += "<p><a href=\"/command?State=R\"><button>Right</button></a></p>";
  html += "<p><a href=\"/command?State=S\"><button>Stop</button></a></p>";
  html += "<p><a href=\"/command?State=O\"><button>Open Bin</button></a></p>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleCommand() {
  String command = server.arg("State");
  if (command == "F" || command == "B" || command == "L" || command == "R" || command == "S" || command == "O") {
    Serial.write(command.charAt(0)); // Send command to ATmega328P via serial
  } else {
    Serial.println("Invalid Command Received");
  }
  server.sendHeader("Location", "/", true);  // Redirect back to the main page
  server.send(302, "text/plain", "");
}

void handleNotFound() {
  server.send(404, "text/plain", "404: Not found");
}

void setup() {
  Serial.begin(115200);  // Initialize serial communication to ATmega328P
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nWiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/command", handleCommand);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
