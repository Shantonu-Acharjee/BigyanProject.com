int rain_pin = 5;
int buzzar_pin = 2;
int Led_pin = 7;
int relay_pin = 4;
void setup() {
  pinMode(rain_pin, INPUT);
  pinMode(buzzar_pin, OUTPUT);
  pinMode(Led_pin, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(relay_pin, OUTPUT);
  digitalWrite(buzzar_pin, LOW);
  digitalWrite(6, LOW);
  digitalWrite(Led_pin, LOW);
  digitalWrite(relay_pin, LOW);
}

void loop() {
  if(!digitalRead(rain_pin)){
    digitalWrite(buzzar_pin, HIGH);
    digitalWrite(Led_pin, HIGH);
    digitalWrite(relay_pin, HIGH);
  }
  else{
    digitalWrite(buzzar_pin, LOW);
    digitalWrite(Led_pin, LOW);
    digitalWrite(relay_pin, LOW);
  }
}
