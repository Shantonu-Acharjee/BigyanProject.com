/* Example sketch to control a stepper motor with TB6560 stepper motor driver, AccelStepper library and Arduino: acceleration and deceleration. More info: https://www.makerguides.com */
// Include the AccelStepper library:
#include <AccelStepper.h>
#include <Servo.h>
// Define stepper motor connections and motor interface type. Motor interface type must be set to 1 when using a driver:
#define dirPin 2
#define stepPin 5
#define motorInterfaceType 1
// Create a new instance of the AccelStepper class:
AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);
void setup() {
// Set the maximum speed and acceleration:
stepper.setMaxSpeed(4000);
stepper.setAcceleration(1000);
}
void loop() {
// Set the target position:
stepper.moveTo(24000);
// Run to target position with set speed and acceleration/deceleration:
stepper.runToPosition();
delay(1000);
// Move back to zero:
stepper.moveTo(0);
stepper.runToPosition();
delay(1000);
}