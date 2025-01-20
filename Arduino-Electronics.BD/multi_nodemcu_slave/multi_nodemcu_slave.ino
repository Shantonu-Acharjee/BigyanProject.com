#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "fbHJJqto7OXojL4GR_xMZLqgw0PMLuLC";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "mobilewifi";
char pass[] = "12345678";

const int RelayPin0 = 16;         // pin for relay output.

void setup()
{
  // Debug console
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  pinMode(RelayPin0, OUTPUT);            //  initialize your pin as output.
  digitalWrite(RelayPin0, HIGH);
}

void loop()
{
  Blynk.run();
}
