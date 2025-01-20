#include "OTA.h"
#include <PubSubClient.h>
const char* mqtt_server = "192.168.0.110";

WiFiClient espClient;
PubSubClient client(espClient);
const int btnLight = 5; //IO5 Light Switch
const int btnFan = 4; //IO4 Fan Switch
const int RelLight = 14; //IO14 Light Relay
const int RelFan = 12; //IO12 Fan Relay
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;
long lastReconnectAttempt = 0; //For the non blocking mqtt reconnect (in millis)
long lastDebounceTime = 0; // Holds the last time debounce was evaluated (in millis).
const int debounceDelay = 80; // The delay threshold for debounce checking.

int onofflight = false; //is relay on or off
int onofffan = false; //is relay on or off
int wantedStateLight = false; //wanted state
int wantedStateFan = false; //wanted state
int debounceState; //internal state for debouncing

char mqttTopicStateLight[64];
char mqttTopicStateFan[64];
char mqttTopicStatus[64];
char mqttTopicDoLight[64];
char mqttTopicDoFan[64];
char mqttTopicIp[64];

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

  if (strcmp(topic,"pir1Status")==0){
    // whatever you want for this topic
  }
  
  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    //digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    //digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
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
    if (client.connect("SarahHome","sweety","1a2s3d4fQA", mqttTopicStatus, 1, true, "offline")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(mqttTopicStatus, "online", true);
      client.publish(mqttTopicStateLight, ((onofflight) ? "1" : "0") , true);
      //client.publish(mqttTopicStateFan, ((onofffan) ? "1" : "0") , true);
      //client.publish(mqttTopicIp, curIp, true);
      // ... and resubscribe
      client.subscribe(mqttTopicDoLight);
      //client.subscribe(mqttTopicDoFan);
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
  Serial.begin(115200);
  ArduinoOTA.setHostname("SarahRoom");
  setupOTA();
  pinMode(btnLight, INPUT_PULLUP);
  pinMode(btnFan, INPUT_PULLUP);
  pinMode(RelLight, OUTPUT);
  pinMode(RelFan, OUTPUT);
  
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  attachInterrupt(btnLight, onChangeButtonLight, CHANGE);
  attachInterrupt(btnFan, onChangeButtonFan, CHANGE);
}

void loop() {
  ArduinoOTA.handle();
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  if (onofflight != wantedStateLight) {
    doOnOffLight();
  }
  if (onofffan != wantedStateFan) {
    doOnOffFan();
  } 
}
void onChangeButtonLight() {

  int reading = digitalRead(btnLight); // Get the pin reading.
  if (reading == debounceState) return; // Ignore dupe readings.

  boolean debounce = false;

  // Check to see if the change is within a debounce delay threshold.
  if ((millis() - lastDebounceTime) <= debounceDelay) {
    debounce = true;
  }

  // This update to the last debounce check is necessary regardless of debounce state.
  lastDebounceTime = millis();

  if (debounce) return; // Ignore reads within a debounce delay threshold.
  debounceState = reading; // All is good, persist the reading as the state.

  if (reading) {
    wantedStateLight = !wantedStateLight;
  }

}

void onChangeButtonFan() {

  int reading = digitalRead(btnFan); // Get the pin reading.
  if (reading == debounceState) return; // Ignore dupe readings.

  boolean debounce = false;

  // Check to see if the change is within a debounce delay threshold.
  if ((millis() - lastDebounceTime) <= debounceDelay) {
    debounce = true;
  }

  // This update to the last debounce check is necessary regardless of debounce state.
  lastDebounceTime = millis();

  if (debounce) return; // Ignore reads within a debounce delay threshold.
  debounceState = reading; // All is good, persist the reading as the state.

  if (reading) {
    wantedStateFan = !wantedStateFan;
  }

}
void doOnOffLight() {
  onofflight = !onofflight;
  Serial.println("new Light state: " + String(onofflight));

  if (onofflight) {
    turnOnLight();
  } else {
    turnOffLight();
  }
}

void doOnOffFan() {
  onofffan = !onofffan;
  Serial.println("new Fan state: " + String(onofffan));

  if (onofffan) {
    turnOnFan();
  } else {
    turnOffFan();
  }
}


void turnOnLight() {
  digitalWrite(RelLight, HIGH);

  client.publish(mqttTopicStateLight, "1", true);

}
void turnOnFan() {
  digitalWrite(RelFan, HIGH);

  //client.publish(mqttTopicStateFan, "1", true);

}

void turnOffLight() {
  digitalWrite(RelLight, LOW);

  client.publish(mqttTopicStateLight, "0", true);
}
void turnOffFan() {
  digitalWrite(RelFan, LOW);

  //client.publish(mqttTopicStateFan, "0", true);
}
