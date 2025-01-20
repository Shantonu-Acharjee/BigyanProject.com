
const int analogOutPin = 9; // Analog output pin that the LED is attached to


void setup() {
}

void loop() {

  analogWrite(analogOutPin, 0);
  Serial.print("\t output = ");
  //delay(2);
}
