#include <SoftwareSerial.h>

// RX-->7, TX-->6 
SoftwareSerial bluetooth(6, 7); // Create software serial object for HC-05 
SoftwareSerial GSM_800L(3, 2); // SIM800L Tx & Rx is connected to Arduino #3 & #2


// Define the pin for the LED
const int ledPinP = 8;
const int ledPinN = 9;
const int alarmPin = 11;
String message = "";
String sms; // Variable to store the SMS message

void setup() {
  
  Serial.begin(9600);// Begin serial communication with Arduino IDE (Serial Monitor)
  bluetooth.begin(9600);// Begin serial communication with the HC-05 module


  // Initialize the LED pin as output
  pinMode(ledPinP, OUTPUT);
  pinMode(ledPinN, OUTPUT);
  pinMode(alarmPin, OUTPUT);

  GSM_800L.begin(9600); // Begin serial communication with Arduino and SIM800L

  Serial.println("Initializing...");
  delay(1000);

  GSM_800L.println("AT"); // Once the handshake test is successful, it will back to OK
  updateSerial();

  GSM_800L.println("AT+CMGF=0"); //Delete all SMS
  updateSerial();
  delay(1000);

  GSM_800L.println("AT+CMGF=1"); // Set SMS to text mode
  updateSerial();

  GSM_800L.println("AT+CMGR=1"); // Read the SMS at index 1 (first message)
  updateSerial();

  
  
}

void loop() {

  // Check if there's any data available from the Bluetooth module
  if (bluetooth.available()) {
    message = bluetooth.readString(); // Read the incoming message
    message.toLowerCase(); // Convert the message to lowercase
    Serial.println(message); // Print the message in lowercase
  }

  if(message == "led on"){
    digitalWrite(ledPinP, HIGH); // Turn the LED on
    digitalWrite(ledPinN, LOW);
    Serial.println("LED ON");
  }

  else if (message == "led off") {
    digitalWrite(ledPinP, LOW); // Turn the LED off
    digitalWrite(ledPinN, LOW);
    Serial.println("LED OFF");
  }


  else if (message == "alarm on") {
    digitalWrite(alarmPin, HIGH); // Turn the LED off
    Serial.println("Alarm On");
  }

  else if (message == "alarm off") {
    digitalWrite(alarmPin, LOW); // Turn the LED off
    Serial.println("Alarm Off");
  }


  message = "";


  pdateSerial(); //Update data for GSM

}//END vOID lOOP







void updateSerial() {
  delay(500);

  while (GSM_800L.available()) {
    String response = GSM_800L.readString(); // Read the incoming message
    Serial.println(response); // Print the response to the Serial Monitor

    // Store the SMS message if it contains text
    if (response.startsWith("+CMGR")) { // Check if the response starts with the expected SMS format
      sms = response; // Store the response in the variable
      Serial.println(sms);

      GSM_800L.println("AT+CMGF=0"); //Delete all SMS
      updateSerial();
    }
  }
}

