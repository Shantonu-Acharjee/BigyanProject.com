#include <Wire.h>

#define SLAVE_ADDRESS 2
char message[100];
char str;
void setup()
{
  Wire.begin(); 
  randomSeed(analogRead(3));
  Serial.begin(9600);  
}

byte x = 0;

void loop()
{
  Serial.println("Requesting Data"); 
  Wire.requestFrom(SLAVE_ADDRESS, 100);
int i = 0;
  while(0 < Wire.available())
  {
    str = Wire.read();
    if(str != char(255)){
    message[i] = str;
    
    i++;
  }
  } 
  
  Serial.println(message); 
  delay(500);
}
