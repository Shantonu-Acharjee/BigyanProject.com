//ESP32Servo by Kevin Harrington, John K. Bennett

#include <ESP32Servo.h> // Include the ESP32 Servo Library
Servo myServo;          // Create a Servo object
int servoPin = 12;      // Define the GPIO pin connected to the servo signal

void setup() {
  Serial.begin(115200); // Initialize serial communication
  myServo.attach(servoPin); // Attach the servo to the specified pin
  Serial.println("Servo Test Initialized");
}

void loop() {
  // Move servo from 0 to 180 degrees
  for (int angle = 0; angle <= 180; angle++) {
    myServo.write(angle);         // Set servo to the current angle
    delay(15);                    // Small delay for smooth movement
  }

  // Move servo back from 180 to 0 degrees
  for (int angle = 180; angle >= 0; angle--) {
    myServo.write(angle);         // Set servo to the current angle
    delay(15);                    // Small delay for smooth movement
  }
}
