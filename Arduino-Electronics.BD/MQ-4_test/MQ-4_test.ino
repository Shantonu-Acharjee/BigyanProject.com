
int value;//For reading Sensor Value;

void setup() {
  Serial.begin(9600);
}

void loop()
{
value= analogRead(A0);//reads the analaog value from the methane sensor's AOUT pin
Serial.println(value);
delay(1000);
}
