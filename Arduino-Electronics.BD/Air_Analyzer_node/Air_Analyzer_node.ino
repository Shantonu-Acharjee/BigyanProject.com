#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPLOvUSS1W8"
#define BLYNK_DEVICE_NAME "IOT Project"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>
#define BLYNK_AUTH_TOKEN "Mg2WNTkH6orfF6DeYk15QjVaY_1uau9O"
char auth[] = "Mg2WNTkH6orfF6DeYk15QjVaY_1uau9O";
SoftwareSerial uno(14,12);

char ssid[] = "mobilewifi";
char pass[] = "abcd1234efgh";
BlynkTimer timer;
String content = "";
String TEM = "";
String HUM = "";
String Alcohol = "";
String Benzene = "";
String Hexane = "";
String CH4 = "";
String smoke = "";
String CO2 = "";
String Toluene = "";
String NH4 = "";
String Acetone = "";
String LPG = "";
String FG = "";
char character;

int ind1; // , locations
int ind2;
int ind3;
int ind4;
int ind5;
int ind6;
int ind7;
int ind8;
int ind9;
int ind10;
int ind11;
int ind12;
int ind13;

void setup()
{
  // Debug console
  Serial.begin(9600);
  uno.begin(9600);
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  Blynk.run();
  timer.run();
  if (uno.available())  {
    char c = uno.read();  //gets one byte from serial buffer
    if (c == '\n') {  
      Serial.println(content);          
      uno.flush();
      ind1 = content.indexOf('=');  //finds location of first ,
      TEM = content.substring(0, ind1);   //captures first data String
      ind2 = content.indexOf('=', ind1+1 );   //finds location of second ,
      HUM = content.substring(ind1+1, ind2);   //captures second data String
      ind3 = content.indexOf('=', ind2+1 );
      Alcohol = content.substring(ind2+1, ind3);
      ind4 = content.indexOf('=', ind3+1 );
      Benzene = content.substring(ind3+1, ind4);
      ind5 = content.indexOf('=', ind4+1 );
      Hexane = content.substring(ind4+1, ind5);
      ind6 = content.indexOf('=', ind5+1 );
      CH4 = content.substring(ind5+1, ind6);
      ind6 = content.indexOf('=', ind6+1 );
      smoke = content.substring(ind6+1, ind7);
      ind6 = content.indexOf('=', ind7+1 );
      CO2 = content.substring(ind7+1, ind8);
      ind6 = content.indexOf('=', ind8+1 );
      Toluene = content.substring(ind8+1, ind9);
      ind6 = content.indexOf('=', ind9+1 );
      NH4 = content.substring(ind9+1, ind10);
      ind6 = content.indexOf('=', ind10+1 );
      Acetone = content.substring(ind10+1, ind11);
      ind6 = content.indexOf('=', ind11+1 );
      LPG = content.substring(ind11+1, ind12);
      ind6 = content.indexOf('=', ind12+1 );
      FG = content.substring(ind12+1);
      content="";
      Blynk.virtualWrite(V0, TEM);
      Blynk.virtualWrite(V1, HUM);
      Blynk.virtualWrite(V2, Alcohol);
      Blynk.virtualWrite(V3, Benzene);
      Blynk.virtualWrite(V4, Hexane);
      Blynk.virtualWrite(V5, CH4);
      Blynk.virtualWrite(V6, smoke);
      Blynk.virtualWrite(V7, CO2);
      Blynk.virtualWrite(V8, Toluene);
      Blynk.virtualWrite(V9, NH4);
      Blynk.virtualWrite(V10, Acetone);
      Blynk.virtualWrite(V11, LPG);
      Blynk.virtualWrite(V12, FG);
    }
    else{
      content += c;
    }
  }
}
void sendSensor()
{
  
}
