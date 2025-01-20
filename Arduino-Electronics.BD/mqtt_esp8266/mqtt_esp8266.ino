/***************************************************
  Adafruit MQTT Library ESP8266 Example

  Must use ESP8266 Arduino from:
    https://github.com/esp8266/Arduino

  Works great with Adafruit's Huzzah ESP board & Feather
  ----> https://www.adafruit.com/product/2471
  ----> https://www.adafruit.com/products/2821

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Tony DiCola for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "Electronics.Com.BD"
#define WLAN_PASS       "abcd1234efgh"

/************************* Server Setup *********************************/

#define AIO_SERVER      "172.16.0.100"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "mosquitto"
#define AIO_KEY         "1a2s3d4fQA."

/************************* Pin Definition *********************************/



/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;
/****************************** Feeds ***************************************/
void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
}

PubSubClient client1(AIO_SERVER, AIO_SERVERPORT, callback, client);

void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.println(F("Adafruit MQTT demo"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  if (client1.connect("arduinoClient", AIO_USERNAME, AIO_KEY)) {
    client1.publish("master-bed/board1/switch1","hello world");
    client1.subscribe("master-bed/board1/switch1");
  }
}

uint32_t x=0;

void loop() {
  client1.loop();
}
