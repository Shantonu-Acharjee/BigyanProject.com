/*
#define BLYNK_TEMPLATE_ID "TMPL3wk2vIcoI"
#define BLYNK_TEMPLATE_NAME "LED Blink"
#define BLYNK_AUTH_TOKEN "aQIL8wCiEW0pLpQQ8XIAiBYywxVTJMJR"
*/


//connect led with DO pin

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define BLYNK_AUTH_TOKEN "aQIL8wCiEW0pLpQQ8XIAiBYywxVTJMJR"

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "shantonu";//Enter your WIFI name " Techiqbal "
char pass[] = "12345678";//Enter your WIFI password " Techiqbal "

//Get the button value
BLYNK_WRITE(V0) {
  digitalWrite(D0, param.asInt());
}

void setup() {
  //Set the LED pin as an output pin
  pinMode(D0, OUTPUT);
  //Initialize the Blynk library
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
}

void loop() {
  //Run the Blynk library
  Blynk.run();
}
