int input=0;
int buzz=3; // Buzzer Pin
int j=3;
int k=12;
void setup() {
pinMode(buzz, OUTPUT);
}

void loop(){
// Whoop up
  for(int hz = 440; hz < 1000; hz+=25){
    tone(buzz, hz, 50);
    delay(5);
    for(int i=3;i<=7;i++)
    digitalWrite(i,HIGH);
  }
  // Whoop down
  for(int hz = 1000; hz > 440; hz-=25){
    tone(buzz, hz, 50);
    delay(5);
    for(int i=3;i<=7;i++)
    {
      digitalWrite(i,LOW);
      digitalWrite(i+5,HIGH);
      }
  }
}
