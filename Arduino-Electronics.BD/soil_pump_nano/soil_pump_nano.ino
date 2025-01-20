int pump=12;
int soil = 11;



void setup() {
  pinMode(pump,OUTPUT);
  pinMode(soil,INPUT);
  digitalWrite(pump,LOW);
}

void loop() {
  if(digitalRead(soil) == LOW){
    digitalWrite(pump,HIGH);
  }
  else{
    digitalWrite(pump,LOW);    
  }

}
