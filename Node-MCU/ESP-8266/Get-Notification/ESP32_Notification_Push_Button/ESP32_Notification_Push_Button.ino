/************************************************************************************

 *  My Channel: https://www.youtube.com/c/TeachMeSomething  Visit for More Project Videos
 *  
 *  *********************************************************************************
 *  Preferences--> Aditional boards Manager URLs : 
 *  For NodeMCU:
 *  https://arduino.esp8266.com/stable/package_esp8266com_index.json
 *  
 *  Preferences--> Aditional boards Manager URLs : 
 *  For ESP32:
 *  https://dl.espressif.com/dl/package_esp32_index.json


 /*
********* ESP8266 (NodeMCU Lolin V3) 30Pin ************

1.Install "CH341SER.EXE" Driver
2.File > Preference > Add This URL "http://arduino.esp8266.com/stable/package_esp8266com_index.json" to Additional Board Manager URLs.
3.Tools > Board > Board Manager > Install "esp8266" by ESP8266 Comunity
4.Tools > Board > esp8266 > NodeMCU 1.0 (ESP-12E Module).
*/
 *  
 *  *********************************************************************************/


/* Fill-in your Template ID (only if using Blynk.Cloud) */

#define BLYNK_TEMPLATE_ID "TMPL3zjaj1x80"
#define BLYNK_TEMPLATE_NAME "Smart Door"
#define BLYNK_AUTH_TOKEN "z-cPWIHXgyP7kxorJPypoARI7A4MdXRK"





#define BLYNK_PRINT Serial

#ifdef ESP32
#include <WiFi.h>
#include <WiFiClient.h>
//==========================================================
char ssid[] = "CAMEL";        // type your wifi name
char pass[] = "8877@6655";  // type your wifi password
String Message="Someone Is Hare!";
//==========================================================

#include <BlynkSimpleEsp32.h>
#define Push_Button 35
#define wifi_led 33
#endif

#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#define Push_Button D1
#define wifi_led D4
#endif

char auth[] = BLYNK_AUTH_TOKEN;

unsigned long old = 0;
unsigned long current = 0;
int interval = 10;
int interval2 = 0;


void setup() {
  Serial.begin(115200);
  Serial.println("\nPlease wait for Blynk Server connection");
  pinMode(wifi_led, OUTPUT);
  pinMode(Push_Button, INPUT);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  wifi_testing();
  Blynk.begin(auth, ssid, pass);
}

void loop() {
  Blynk.run();
  wifi_testing();
  current = millis();
  interval2 = (current - old) / 1000;
  if ((digitalRead(Push_Button) == LOW) & (interval2 >= 3)) {
    Serial.println("Blynk notification Sent to Mobile");
    Blynk.logEvent("notification", Message); //-------------------------------->
    while (digitalRead(Push_Button) == LOW)
    delay(10);
    current = millis();
    old = millis();
    interval2 = 0;
  }
}

void wifi_testing() {
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
    digitalWrite(wifi_led, LOW);
    delay(250);
    digitalWrite(wifi_led, HIGH);
    current = millis();
    if ((current - old) / 1000 > interval)
      ESP.restart();
  }
}
