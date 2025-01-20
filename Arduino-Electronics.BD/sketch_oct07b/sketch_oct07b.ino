#include <AFMotor.h>
#include <Servo.h>

// Create AF_DCMotor objects for motors M1 and M4
AF_DCMotor motor1(1);
AF_DCMotor motor2(4);

Servo binServo;  // Create a Servo object for bin cap

void setup() {
  Serial.begin(115200);  // Initialize serial communication
  Serial.println("Motor Control Ready");

  // Set the motor speeds (between 0 and 255)
  motor1.setSpeed(0);  // Motor initially off
  motor2.setSpeed(0);  // Motor initially off

  binServo.attach(10);  // Attach servo to pin 10 for controlling bin cap
  closeBin();           // Ensure the bin is initially closed
}

void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read();
    if (command == 'F') {
      moveForward();
    } else if (command == 'B') {
      motor1.setSpeed(0);  // Turn off motor
      motor2.setSpeed(0);  // Turn off motor
      moveBackward();
    } else if (command == 'L') {
      motor1.setSpeed(0);  // Turn off motor
      motor2.setSpeed(0);  // Turn off motor
      turnLeft();
    } else if (command == 'R') {
      motor1.setSpeed(0);  // Turn off motor
      motor2.setSpeed(0);  // Turn off motor
      turnRight();
    } else if (command == 'S') {
      motor1.setSpeed(0);  // Turn off motor
      motor2.setSpeed(0);  // Turn off motor
      Serial.println("Motors Stopped");
    } else if (command == 'O') {
      openBin();
    }
  }
}

void moveForward() {
  motor1.setSpeed(150);
  motor2.setSpeed(150);
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  Serial.println("Motors Forward");
}

void moveBackward() {
  motor1.setSpeed(150);
  motor2.setSpeed(150);
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  Serial.println("Motors Backward");
}

void turnLeft() {
  motor1.setSpeed(150);
  motor2.setSpeed(150);
  motor1.run(BACKWARD);
  motor2.run(FORWARD);
  Serial.println("Turn Left");
  delay(500);          // Turn for 0.5 seconds
  motor1.setSpeed(0);  // Turn off motor
  motor2.setSpeed(0);  // Turn off motor
}

void turnRight() {
  motor1.setSpeed(150);
  motor2.setSpeed(150);
  motor1.run(FORWARD);
  motor2.run(BACKWARD);
  Serial.println("Turn Right");
  delay(500);          // Turn for 0.5 seconds
  motor1.setSpeed(0);  // Turn off motor
  motor2.setSpeed(0);  // Turn off motor
}

void openBin() {
  binServo.write(90);  // Open the bin cap (adjust the angle as needed)
  Serial.println("Bin Cap Opened");
  delay(3000);        // Keep the cap open for 3 seconds
  binServo.write(0);  // Close the bin cap
  Serial.println("Bin Cap Closed");
}

void closeBin() {
  binServo.write(0);  // Close the bin cap initially
  Serial.println("Bin Cap Closed Initially");
}
