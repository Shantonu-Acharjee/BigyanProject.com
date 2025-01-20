#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPLJNlQv-9b"
#define BLYNK_DEVICE_NAME "IOT Irrigation"
#define BLYNK_AUTH_TOKEN "6rS8KWlwAws3PFgkn90wP9g9bUOYmrm6"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>
char auth[] = "6rS8KWlwAws3PFgkn90wP9g9bUOYmrm6";
SoftwareSerial nano(14,12);

char ssid[] = "mobilewifi";
char pass[] = "abcd1234efgh";
String content = "";
String PUMP = "";
char character;

int ind1; // , locations
int ind2;
int ind3;



BLYNK_CONNECTED()
{
  Blynk.syncVirtual(V0);  
     
  Blynk.syncVirtual(V1);
  Blynk.syncVirtual(V2);
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
BLYNK_WRITE(V0)
{
  if(param.asInt()==1){
    digitalWrite(5, HIGH);
  }
  else{
    digitalWrite(5, LOW);
  }
}
void setup()
{
  // Debug console
  Serial.begin(115200);
  nano.begin(115200);
  pinMode(5,OUTPUT);
  pinMode(4,OUTPUT);
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  Blynk.run();
  if (nano.available())  {
    
    char c = nano.read();  //gets one byte from serial buffer
    //Serial.println(c);         
      nano.flush();
      PUMP = String(c);
      PUMP.trim();
      
      Serial.println(PUMP); 
      Blynk.virtualWrite(V0, PUMP);
      if(PUMP == "1")
      Blynk.virtualWrite(V1, "PUMP Running");
      else
      Blynk.virtualWrite(V1, "PUMP Stoped");
    
  }
}
