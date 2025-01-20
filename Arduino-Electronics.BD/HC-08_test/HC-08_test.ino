#include <SoftwareSerial.h>
 
char sms;
SoftwareSerial BT(3, 4);
void setup() 
{
  Serial.begin(9600);
  BT.begin(9600);
}
  
 
 
void loop() 
{  
  Serial.print(" SMS=");
  Serial.print(sms);
  Serial.println();
  if (BT.available()) 
  {
    sms=BT.read();
  }
}
