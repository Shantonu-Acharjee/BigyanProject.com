#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#else
#error "Board not found"
#endif
#include <PubSubClient.h>
#include <SoftwareSerial.h>
#define BUILTIN_LED 2
// Update these with values suitable for your network.

const char* ssid = "Electronics2g";
const char* password = "abcd1234efgh";
const char* mqtt_server = "172.16.0.100";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;
int latchPin = D7;  //--> Pin connected to ST_CP of 74HC595
int clockPin = D5;  //--> Pin connected to SH_CP of 74HC595
int dataPin = D6;   //--> Pin connected to DS of 74HC595

byte SR4LEDS = 0;  //--> The byte variable to be entered into the Second 
byte SR3LEDS = 0;  //--> The byte variable to be entered into the Second 
byte SR2LEDS = 0;  //--> The byte variable to be entered into the Second 74HC595
byte SR1LEDS = 0;  //--> The byte variable to be entered into the First 74HC595
void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '0' && (char)payload[1] == '1') { // popcorn
    SR1LEDS = B10000000;
    SR2LEDS = B00000000;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50); 
  } else if ((char)payload[0] == '0' && (char)payload[1] == '2') { // Pizza
    SR1LEDS = B01000000;
    SR2LEDS = B00000000;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  } else if ((char)payload[0] == '0' && (char)payload[1] == '3') { // Potato
    SR1LEDS = B00100000;
    SR2LEDS = B00000000;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  } else if ((char)payload[0] == '0' && (char)payload[1] == '4') { // Re Heat
    SR1LEDS = B00010000;
    SR2LEDS = B00000000;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  } else if ((char)payload[0] == '0' && (char)payload[1] == '5') { // Spaghety
    SR1LEDS = B00001000;
    SR2LEDS = B00000000;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  } else if ((char)payload[0] == '0' && (char)payload[1] == '6') { // Cake
    SR1LEDS = B00000100;
    SR2LEDS = B00000000;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  } else if ((char)payload[0] == '0' && (char)payload[1] == '7') { // Beverage
    SR1LEDS = B00000010;
    SR2LEDS = B00000000;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  } else if ((char)payload[0] == '0' && (char)payload[1] == '8') { // Chicken
    SR1LEDS = B00000001;
    SR2LEDS = B00000000;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  } else if ((char)payload[0] == '0' && (char)payload[1] == '9') { // 1
    SR1LEDS = B00000000;
    SR2LEDS = B10000000;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  } else if ((char)payload[0] == '1' && (char)payload[1] == '0') { // 2
    SR1LEDS = B00000000;
    SR2LEDS = B01000000;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  } else if ((char)payload[0] == '1' && (char)payload[1] == '1') { // 3
    SR1LEDS = B00000000;
    SR2LEDS = B00100000;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  } else if ((char)payload[0] == '1' && (char)payload[1] == '2') { // 4
    SR1LEDS = B00000000;
    SR2LEDS = B00010000;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  } else if ((char)payload[0] == '1' && (char)payload[1] == '3') { // 5
    SR1LEDS = B00000000;
    SR2LEDS = B00001000;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  } else if ((char)payload[0] == '1' && (char)payload[1] == '4') { // 6
    SR1LEDS = B00000000;
    SR2LEDS = B00000100;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  } else if ((char)payload[0] == '1' && (char)payload[1] == '5') { // 7
    SR1LEDS = B00000000;
    SR2LEDS = B00000010;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  } else if ((char)payload[0] == '1' && (char)payload[1] == '6') { // 8
    SR1LEDS = B00000000;
    SR2LEDS = B00000001;
    SR3LEDS = B00000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  } else if ((char)payload[0] == '1' && (char)payload[1] == '7') { // 9
    SR1LEDS = B00000000;
    SR2LEDS = B00000000;
    SR3LEDS = B10000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  } else if ((char)payload[0] == '1' && (char)payload[1] == '8') { // Weight
    SR1LEDS = B00000000;
    SR2LEDS = B00000000;
    SR3LEDS = B01000000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  } else if ((char)payload[0] == '1' && (char)payload[1] == '9') { // 0
    SR1LEDS = B00000000;
    SR2LEDS = B00000000;
    SR3LEDS = B00100000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  } else if ((char)payload[0] == '2' && (char)payload[1] == '0') { // Time
    SR1LEDS = B00000000;
    SR2LEDS = B00000000;
    SR3LEDS = B00010000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  }else if ((char)payload[0] == '2' && (char)payload[1] == '1') { // Power Le
    SR1LEDS = B00000000;
    SR2LEDS = B00000000;
    SR3LEDS = B00001000;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  }else if ((char)payload[0] == '2' && (char)payload[1] == '2') { // Grill
    SR1LEDS = B00000000;
    SR2LEDS = B00000000;
    SR3LEDS = B00000100;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  } else if ((char)payload[0] == '2' && (char)payload[1] == '3') { // Micro
    SR1LEDS = B00000000;
    SR2LEDS = B00000000;
    SR3LEDS = B00000010;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  } else if ((char)payload[0] == '2' && (char)payload[1] == '4') {// Convenction
    SR1LEDS = B00000000;
    SR2LEDS = B00000000;
    SR3LEDS = B00000001;
    SR4LEDS = B00000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  } else if ((char)payload[0] == '2' && (char)payload[1] == '5') { // Micro con
    SR1LEDS = B00000000;
    SR2LEDS = B00000000;
    SR3LEDS = B00000000;
    SR4LEDS = B10000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  } else if ((char)payload[0] == '2' && (char)payload[1] == '6') { // QuickStart
    SR1LEDS = B00000000;
    SR2LEDS = B00000000;
    SR3LEDS = B00000000;
    SR4LEDS = B01000000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  } else if ((char)payload[0] == '2' && (char)payload[1] == '7') { // Stop
    SR1LEDS = B00000000;
    SR2LEDS = B00000000;
    SR3LEDS = B00000000;
    SR4LEDS = B00100000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  } else if ((char)payload[0] == '2' && (char)payload[1] == '8') { // Start
    SR1LEDS = B00000000;
    SR2LEDS = B00000000;
    SR3LEDS = B00000000;
    SR4LEDS = B00010000;
    updateShiftRegister(SR1LEDS, SR2LEDS, SR3LEDS, SR4LEDS);
    delay(50);
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("ECOAC/Remote", "hello world");
      // ... and resubscribe
      client.subscribe("ECOAC/Remote");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  /*unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    snprintf (msg, MSG_BUFFER_SIZE, "hello world #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("ECOAC/Remote", msg);
  }*/
}
void updateShiftRegister(byte SR1, byte SR2, byte SR3, byte SR4)  {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, SR4);
  shiftOut(dataPin, clockPin, LSBFIRST, SR3);
  shiftOut(dataPin, clockPin, LSBFIRST, SR2);
  shiftOut(dataPin, clockPin, LSBFIRST, SR1);
  digitalWrite(latchPin, HIGH);
}
