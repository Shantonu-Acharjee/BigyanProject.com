#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPLivcRX8xw"
#define BLYNK_DEVICE_NAME "Incubator"
#define BLYNK_AUTH_TOKEN "ar93salWAFWaPtXx2O7veG6DUIR5OwAq"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>
char auth[] = "ar93salWAFWaPtXx2O7veG6DUIR5OwAq";
SoftwareSerial nano(14,12);

char ssid[] = "mobilewifi";
char pass[] = "abcd1234efgh";
String content = "";
String HUM = "";
String TEM = "";
String SMO = "";
char character;

int ind1; // , locations
int ind2;
int ind3;



BLYNK_CONNECTED()
{
  Blynk.syncVirtual(V1);  

  Blynk.syncVirtual(V0);  

  Blynk.syncVirtual(V2);   
}

void setup()
{
  // Debug console
  Serial.begin(115200);
  nano.begin(9600);
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  Blynk.run();
  if (nano.available())  {
    char c = nano.read();  //gets one byte from serial buffer
    if (c == '\n') {  
      Serial.println(content);          
      nano.flush();
      ind1 = content.indexOf('=');  //finds location of first ,
      HUM = content.substring(0, ind1);   //captures first data String
      ind2 = content.indexOf('=', ind1+1 );   //finds location of second ,
      TEM = content.substring(ind1+1, ind2);   //captures second data String
      ind3 = content.indexOf('=', ind2+1 );
      SMO = content.substring(ind2+1);
      SMO.trim();
      content="";
      
      Serial.println(SMO); 
      Blynk.virtualWrite(V0, HUM);
      Blynk.virtualWrite(V1, TEM);
      Blynk.virtualWrite(V2, SMO);
      
    }
    else{
      content += c;
    }
  }
}
