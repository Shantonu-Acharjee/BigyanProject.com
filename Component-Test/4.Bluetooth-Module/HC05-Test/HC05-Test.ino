char Incoming_value = 0;
void setup() {
  // put your setup code  here, to run once:
Serial.begin(9600);
pinMode(11,OUTPUT);
}

void  loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()  > 0)
    {
      Incoming_value = Serial.read();
      Serial.println(Incoming_value);
      
      if (Incoming_value == '1')
        digitalWrite(11,HIGH);
      else if(Incoming_value == '0')
        digitalWrite(11,LOW);
    }
}
