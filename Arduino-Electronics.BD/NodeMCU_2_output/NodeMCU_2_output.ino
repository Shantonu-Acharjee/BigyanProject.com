#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "c3degTVhd59pnc8KFuu3SIxO6gq3eN-p";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Sarah";
char pass[] = "abcd1234efgh";

void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  Blynk.run();
}
