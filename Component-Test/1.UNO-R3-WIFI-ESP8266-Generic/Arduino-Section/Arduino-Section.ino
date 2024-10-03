/*
  ESP8266 Blink
  Blink the LED on the ESP8266 module
  This example code is in the public domain

  The external LED on the ESP8266 module is connected to GPIO12
 
*/

void setup() {
  pinMode(12, OUTPUT);     // Initialize pin 12 which is GPIO12 as an output
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(12, LOW);   // Turn the LED off
  // low is off on the ESP8266-12
  delay(1000);                      // Wait for a second
  digitalWrite(12, HIGH);           // Turn the LED on by making the voltage HIGH
  delay(2000);                      // Wait for two seconds (to distinguish between low and high)
}