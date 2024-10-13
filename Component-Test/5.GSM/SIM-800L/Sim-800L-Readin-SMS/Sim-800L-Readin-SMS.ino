#include <SoftwareSerial.h>

String latestSMS = ""; // Variable to store the latest SMS message

// Create software serial object to communicate with SIM800L
SoftwareSerial mySerial(3, 2); // SIM800L Tx & Rx is connected to Arduino #3 & #2

void setup() {
  // Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);

  // Begin serial communication with Arduino and SIM800L
  mySerial.begin(9600);

  Serial.println("Initializing..."); 
  delay(1000);

  mySerial.println("AT"); // Handshake test
  updateSerial();

  mySerial.println("AT+CMGF=0"); // Delete all SMS
  updateSerial();
  delay(1000);

  mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();

  mySerial.println("AT+CNMI=2,2,0,0,0"); // Forward incoming messages to the terminal
  updateSerial();
}

void loop() {
  if (mySerial.available()) {
    String response = mySerial.readString(); // Read the entire incoming message
    Serial.println("Full response: " + response); // Print the full response (for debugging)

    // Check if the response contains the SMS header and extract the actual message
    int indexOfMessage = response.indexOf("\n", response.indexOf("+CMT:"));
    
    if (indexOfMessage >= 0) {
      latestSMS = response.substring(indexOfMessage + 1); // Extract the actual SMS text
      latestSMS.trim(); // Remove any leading/trailing whitespace

      // If there is still metadata in the message, remove it
      int secondNewline = latestSMS.indexOf("\n");
      if (secondNewline >= 0) {
        latestSMS = latestSMS.substring(secondNewline + 1); // Get only the SMS content after the second newline
        latestSMS.trim(); // Remove any trailing whitespace again
      }

      Serial.println("Latest SMS: " + latestSMS); // Print only the latest SMS
    }
  }
}

void updateSerial() {
  delay(500);
  while (Serial.available()) {
    mySerial.write(Serial.read()); // Forward data from Serial to SIM800L
  }
  while (mySerial.available()) {
    Serial.write(mySerial.read()); // Forward data from SIM800L to Serial
  }
}
