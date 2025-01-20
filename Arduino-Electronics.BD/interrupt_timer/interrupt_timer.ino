const int ledPin =  13;
const int interruptPin = 2;
int ledState = LOW;
unsigned long previousMillis = 0;
unsigned long interval = 1000;
unsigned int flag = 0;
void setup() {  
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  digitalWrite (ledPin, LOW);
  previousMillis = millis();
  attachInterrupt(digitalPinToInterrupt(interruptPin), buttonPressed, LOW);
}
void loop() {
  unsigned long currentMillis = millis();

  if ((currentMillis - previousMillis <= interval) && ledState == HIGH) {
      digitalWrite(ledPin, ledState);
  } 
  else{
    interval = 0;
    ledState = LOW;
    digitalWrite(ledPin, ledState);
  }
}

void buttonPressed()
{  
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > 200)
  {
    if(interval == 0){
      previousMillis = millis();    
    }
    interval = interval + 5000;
    last_interrupt_time = interrupt_time;
    ledState = HIGH;
  }
}
