//Board --> Generic ESP8266 Module


void setup() {
  pinMode(2, OUTPUT);  
}

void loop() {
  digitalWrite(2, LOW);  
  delay(1000);                      
  digitalWrite(2, HIGH);  
  delay(2000);                 
}
