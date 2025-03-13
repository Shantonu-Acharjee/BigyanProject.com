/*
********* ESP8266 (NodeMCU Lolin V3)30Pin ************

1.Install "CH341SER.EXE" Driver
2.File > Preference > Add This URL "http://arduino.esp8266.com/stable/package_esp8266com_index.json" to Additional Board Manager URLs.
3.Tools > Board > Board Manager > Install "esp8266" by ESP8266 Comunity
4.Tools > Board > esp8266 > NodeMCU 1.0 (ESP-12E Module).
*/




#define LEDpin D4   //set led pin - this is inbuild led


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LEDpin, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LEDpin, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LEDpin, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}