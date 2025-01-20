int relay_1 = 2;
int relay_2 = 3;
int btn = 4;
void setup() {
  pinMode(relay_1,OUTPUT);
  pinMode(relay_2,OUTPUT);
  pinMode(btn,INPUT);
}

void loop() {
  if(digitalRead(btn) == HIGH){
    digitalWrite(relay_1,LOW);
    digitalWrite(relay_2,HIGH);
  }
  else{
    digitalWrite(relay_1,HIGH);
    digitalWrite(relay_2,LOW);
    
  }

}
