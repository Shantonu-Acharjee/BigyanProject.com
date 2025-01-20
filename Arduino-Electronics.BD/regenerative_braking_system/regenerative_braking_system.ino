#include <SoftwareSerial.h>
int relay = 13; 
char sms;
SoftwareSerial BT(2,3);
void setup() 
{
  Serial.begin(9600);
  BT.begin(9600);
  pinMode(relay,OUTPUT);
}
  
 
 
void loop() 
{  
  sms = 'D';
  if (BT.available()) 
  {
    sms=BT.read();
  }
  if(sms=='F')
  {
    Serial.print(" SMS=");
    Serial.print(sms);
    Serial.println();
    digitalWrite(relay,HIGH);
  }
  if(sms=='S'){
    Serial.print(" SMS=");
    Serial.print(sms);
    Serial.println();
    digitalWrite(relay,LOW);
  }
}
