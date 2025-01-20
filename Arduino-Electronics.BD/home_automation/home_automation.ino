#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#define BLYNK_TEMPLATE_ID "TMPLEgU9pqXr"
#define BLYNK_DEVICE_NAME "IOT Project"
#define BLYNK_AUTH_TOKEN "mZvemH4yXu-TmXwh4xPk43WOI4BAuwp7"

char auth[] = "mZvemH4yXu-TmXwh4xPk43WOI4BAuwp7";

char ssid[] = "mobilewifi";
char pass[] = "abcd1234efgh";

BLYNK_WRITE(V0)
{
  if(param.asInt()==1){
    digitalWrite(16, HIGH);
  }
  else{
    digitalWrite(16, LOW);
  }
}
BLYNK_WRITE(V1)
{
  if(param.asInt()==1){
    digitalWrite(13, HIGH);
  }
  else{
    digitalWrite(13, LOW);
  }
}
BLYNK_WRITE(V2)
{
  if(param.asInt()==1){
    digitalWrite(4, HIGH);
  }
  else{
    digitalWrite(4, LOW);
  }
}
BLYNK_WRITE(V3)
{
  if(param.asInt()==1){
    digitalWrite(5, HIGH);
  }
  else{
    digitalWrite(5, LOW);
  }
}

BLYNK_CONNECTED()
{
  Blynk.syncVirtual(V0);  

  Blynk.syncVirtual(V1);  

  Blynk.syncVirtual(V2);  

  Blynk.syncVirtual(V3);  
}

void setup()
{
  pinMode(16, OUTPUT); // Initialise digital pin 2 as an output pin
  pinMode(13, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  Serial.begin(9600);
  delay(100);

  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  Blynk.run();
}
