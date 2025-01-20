#include <SoftwareSerial.h>
SoftwareSerial GPRS(7, 8);
#define PIR_sensor 10
const int buzzerPin=9;
const int LEDPin=13;
int stat=0;
void setup()
{
  pinMode(PIR_sensor, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(LEDPin, OUTPUT);  
  digitalWrite(PIR_sensor, HIGH);
  GPRS.begin(9600);
  Serial.begin(9600);  
  GPRS.println("AT+CMGF=1");  
  delay(2000);
}
void loop()
{ 
  if(digitalRead(PIR_sensor) == LOW)
  {
    if(stat == 1)
    {
      stat = 0;
      tone(buzzerPin,3000);
      digitalWrite(LEDPin, HIGH);      
      GPRS.println("AT+CMGS=\"+8801733789344\"");    
      delay(500);      
      GPRS.print("Movement Detected ");
      GPRS.write( 0x1a ); // ctrl+Z character     
      delay(500);
      GPRS.println();   
      delay(500);
      GPRS.println("AT+CMGS=\"+8801678399558\"");     
      delay(500);     
      GPRS.print("Movement Detected ");
      GPRS.write( 0x1a ); // ctrl+Z character    
      delay(500);     
      GPRS.println(); 
      delay(2000);
    }
  }
  else 
  {
    stat = 1;
    digitalWrite(LEDPin, LOW);
    noTone(buzzerPin);
  }
}
