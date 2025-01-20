int buzzer=13;
int mq = 2;



void setup() {
  pinMode(buzzer,OUTPUT);
  pinMode(mq,INPUT);
  digitalWrite(buzzer,LOW);
}

void loop() {
  if(digitalRead(mq) == LOW){
    digitalWrite(buzzer,HIGH);
    delay(100);
    digitalWrite(buzzer,LOW);
    delay(100);
  }
  else{
    digitalWrite(buzzer,LOW);    
  }

}
