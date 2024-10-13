#include <SoftwareSerial.h>

// Create software serial object for HC-05 
SoftwareSerial bluetooth(6, 7); // RX-->7, TX-->6




void setup() {
  
  Serial.begin(9600);// Begin serial communication with Arduino IDE (Serial Monitor)
  bluetooth.begin(9600);// Begin serial communication with the HC-05 module

}

void loop() {

  // Check if there's any data available from the Bluetooth module
  if (bluetooth.available()) {
    String message = bluetooth.readString(); // Read the incoming message
    message.toLowerCase(); // Convert the message to lowercase
    Serial.println(message); // Print the message in lowercase
  }

  
}
