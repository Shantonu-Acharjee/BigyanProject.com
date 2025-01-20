#define PIR_sensor 10
const int buzzerPin=9;
const int LEDPin=13;
int stat=0;
void setup()
{
  pinMode(PIR_sensor, INPUT);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);   
  delay(2000);
}
void loop()
{ 
  Serial.write(digitalRead(PIR_sensor));
  if(digitalRead(PIR_sensor))
  {
    
  }
  else 
  {
    stat = 1;
    digitalWrite(LEDPin, LOW);
    noTone(buzzerPin);
  }
}
