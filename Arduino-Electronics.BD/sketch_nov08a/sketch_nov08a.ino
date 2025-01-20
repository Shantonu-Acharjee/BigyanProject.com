
#include <SoftwareSerial.h>

SoftwareSerial bluetooth(2, 3);

void setup() {
  //Begin Serial Monitor
  Serial.begin(9600);
  bluetooth.begin(9600);
}


void loop() {
  char received = 'z';   //Received Command from bluetooth

  if (bluetooth.available()) { //If some thing is received by bluetooth
    received = bluetooth.read(); //Read the serial buffer
    delay(10);
    Serial.println(received);//Print it on the serial monitor of arduino
  }
}
