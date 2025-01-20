#include <UIPEthernet.h>
#include "PubSubClient.h"
#include "DHT.h"

#define CLIENT_ID       "UnoMQTT"
#define INTERVAL        3000 // 3 sec delay between publishing
#define DHTPIN          3
#define DHTTYPE         DHT11
bool statusKD=HIGH;//living room door
bool statusBD=HIGH;//front door
bool statusGD=HIGH;//garage door
int lichtstatus;
uint8_t mac[6] = {0x00,0x01,0x02,0x03,0x04,0x05};

EthernetClient ethClient;
PubSubClient mqttClient;
DHT dht(DHTPIN, DHTTYPE);

long previousMillis;

void setup() {
pinMode(4,INPUT_PULLUP);
pinMode(5,INPUT_PULLUP);
pinMode(6,INPUT_PULLUP);
  // setup serial communication
  //Serial.begin(9600);
  // setup ethernet communication using DHCP
  if(Ethernet.begin(mac) == 0) {
    //Serial.println(F("Ethernet configuration using DHCP failed"));
    for(;;);
  }
  // setup mqtt client
  mqttClient.setClient(ethClient);
  mqttClient.setServer("test.mosquitto.org",1883);
  //mqttClient.setServer("192.168.1.xxx",1883); //for using local broker
  //mqttClient.setServer("broker.hivemq.com",1883);
  //Serial.println(F("MQTT client configured"));

  // setup DHT sensor
  dht.begin();
  previousMillis = millis();
}

void loop() {
  statusBD=digitalRead(4);
  statusGD=digitalRead(5);
  statusKD=digitalRead(6);
  lichtstatus = analogRead(A0);
  // check interval
  if(millis() - previousMillis > INTERVAL) {
    sendData();
    previousMillis = millis();
  }
  mqttC
