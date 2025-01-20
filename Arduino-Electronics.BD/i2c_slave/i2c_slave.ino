#include <Wire.h>

char message[100];
#define SLAVE_ADDRESS 0x60
byte x = 0x00;
void setup()
{
  Wire.begin(SLAVE_ADDRESS);
  Wire.onRequest(requestEvent);
  Serial.begin(9600);
}

void loop()
{
  delay(100);
}

void requestEvent() 
{
  sprintf(message, "x=%d&y=%d&t=%d&b=%d&s=%d", 2, 3,4,5,6); // size of this string can vary from 10 up to 15
  Wire.write(message);
}
