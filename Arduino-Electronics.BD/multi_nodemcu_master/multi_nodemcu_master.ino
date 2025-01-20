#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "83PKIaqQ8smHX9A323luzawk70LiPQWZ";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "mobilewifi";
char pass[] = "12345678";

WidgetBridge bridge1(V2);
BlynkTimer timer;
const int btnPin0 = 5;             // pin for physical push button switch.
const int RelayPin0 = 16;         // pin for relay output.
int btnPinState0 = LOW;           // ON
int RelayPinState0 = HIGH;        // OFF

BLYNK_WRITE(V0)
{                           
  RelayPinState0 = param.asInt();
  bridge1.digitalWrite(RelayPin0, RelayPinState0);
}

BLYNK_CONNECTED() {
  bridge1.setAuthToken("fbHJJqto7OXojL4GR_xMZLqgw0PMLuLC"); // Place the AuthToken of the second hardware here
  Blynk.syncVirtual(V0);
}
void blynkAnotherDevice() // Here we will send HIGH or LOW once per second
{  
  if (digitalRead(btnPin0) == LOW)
  {
    if (btnPinState0 != LOW)
    {
      RelayPinState0 = !RelayPinState0;
      bridge1.digitalWrite(RelayPin0, RelayPinState0);
      Blynk.virtualWrite(V0, RelayPinState0);        
    }
    btnPinState0 = LOW; 
  }
  else
  {
    btnPinState0 = HIGH;
  }
 
}
void setup()
{
  // Debug console
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  pinMode(btnPin0, INPUT_PULLUP);        //  initialize your pin as input with enable internal pull-up resistor "input_pullup" 
  timer.setInterval(100L, blynkAnotherDevice); 
}

void loop()
{
  Blynk.run();
  timer.run();
}
