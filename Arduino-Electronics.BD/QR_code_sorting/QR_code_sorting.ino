/* Example sketch to control a stepper motor with TB6560 stepper motor driver, AccelStepper library and Arduino: acceleration and deceleration. More info: https://www.makerguides.com */
// Include the AccelStepper library:
#include <AccelStepper.h>
#include <Servo.h>
// Define stepper motor connections and motor interface type. Motor interface type must be set to 1 when using a driver:
#define limitPinX 3
#define limitPinY 12
#define servoPinX 4
#define servoPinY 5
#define enablePinX 6
#define dirPinX 7
#define stepPinX 8
#define enablePinY 9
#define dirPinY 10
#define stepPinY 11
#define CameraPin 13
#define motorInterfaceType 1
int pos = 0;
long initial_homing=1;
long X_homing=1;
long Y_homing=1;
int Object = 0;
// Create a new instance of the AccelStepper class:
AccelStepper stepperX = AccelStepper(motorInterfaceType, stepPinX, dirPinX);
AccelStepper stepperY = AccelStepper(motorInterfaceType, stepPinY, dirPinY);
Servo servoX;  // create servo object to control a servo
Servo servoY;  // create servo object to control a servo
void setup() {
  Serial.begin(115200); 
  Serial1.begin(115200);
// Set the maximum speed and acceleration:
  stepperX.setMaxSpeed(1000);
  stepperX.setAcceleration(1000);
  stepperY.setMaxSpeed(1000);
  stepperY.setAcceleration(1000);
  servoX.attach(servoPinX);
  servoY.attach(servoPinY);
  pinMode(limitPinX, INPUT_PULLUP);
  pinMode(limitPinY, INPUT_PULLUP);
  pinMode(CameraPin, OUTPUT);
  servoX.write(0);
  servoY.write(5);
  while (digitalRead(limitPinX)) {  // Make the Stepper move CCW until the switch is activated   
      stepperX.moveTo(initial_homing);  // Set the position to move to
      initial_homing++;  // Decrease by 1 for next move if needed
      stepperX.run();  // Start moving the stepper
      Serial.println("backward_x");
  }
  stepperX.setCurrentPosition(0);
  stepperX.setMaxSpeed(100);
  stepperX.setAcceleration(100);
  initial_homing=-1;
  while (!digitalRead(limitPinX)) { // Make the Stepper move CW until the switch is deactivated
    stepperX.moveTo(initial_homing);  
    stepperX.run();
    initial_homing--;
    delay(5);
    Serial.println("forward_x");
  }
  initial_homing=1;
  while (digitalRead(limitPinY)) {  // Make the Stepper move CCW until the switch is activated   
      stepperY.moveTo(initial_homing);  // Set the position to move to
      initial_homing++;  // Decrease by 1 for next move if needed
      stepperY.run();  // Start moving the stepper
      Serial.println("backward_y");
  }
  stepperY.setCurrentPosition(0);
  stepperY.setMaxSpeed(100);
  stepperY.setAcceleration(100);
  initial_homing=-1;
  while (!digitalRead(limitPinY)) { // Make the Stepper move CW until the switch is deactivated
    stepperY.moveTo(initial_homing);  
    stepperY.run();
    initial_homing--;
    delay(5);
    Serial.println("forward_y");
  }
}
void loop() {
  digitalWrite(CameraPin,HIGH);
  while (Serial1.available()>0)  {
    digitalWrite(CameraPin,LOW);
    Object = Serial1.parseInt();
    if(Object == 1001){
      servoX_ACW();
      forward_x();
      forward_y(-5000);
      servoY_ACW();
      delay(2000);
      servoY_CW();
      backward_x();
      backward_y();
      servoX_CW();
    } 
    else if(Object == 1002){
      servoX_ACW();
      forward_x();
      forward_y(-11500);
      servoY_ACW();
      delay(2000);
      servoY_CW();
      backward_x();
      backward_y();
      servoX_CW();
    }
    else if(Object == 1003){
      servoX_ACW();
      forward_x();
      forward_y(-18000);
      servoY_ACW();
      delay(2000);
      servoY_CW();
      backward_x();
      backward_y();
      servoX_CW();
    }   
    else{
      Serial.println(Object);
    }
  }
  
}
void home(){

}
void forward_x(){
  stepperX.setMaxSpeed(4000);
  stepperX.setAcceleration(1000);
  Serial.println("forward_x");
  // Set the target position:
  stepperX.moveTo(-23500);
  // Run to target position with set speed and acceleration/deceleration:
  stepperX.runToPosition();
}
void forward_y(int positionY){
  stepperY.setMaxSpeed(2000);
  stepperY.setAcceleration(1000);
  // Set the target position:
  stepperY.moveTo(positionY);
  // Run to target position with set speed and acceleration/deceleration:
  stepperY.runToPosition();
}
void backward_x(){
  X_homing=1;
  while (digitalRead(limitPinX)) {  // Make the Stepper move CCW until the switch is activated   
      stepperX.moveTo(X_homing);  // Set the position to move to
      X_homing++;  // Decrease by 1 for next move if needed
      stepperX.run();  // Start moving the stepper
      Serial.println("backward_x");
  }
  stepperX.setCurrentPosition(0);
  stepperX.setMaxSpeed(100);
  stepperX.setAcceleration(100);
  X_homing=-1;
  while (!digitalRead(limitPinX)) { // Make the Stepper move CW until the switch is deactivated
    stepperX.moveTo(X_homing);  
    stepperX.run();
    X_homing--;
    delay(5);
    Serial.println("forward_x");
  }
}
void backward_y(){
  Y_homing=1;
  while (digitalRead(limitPinY)) {  // Make the Stepper move CCW until the switch is activated   
      stepperY.moveTo(Y_homing);  // Set the position to move to
      Y_homing++;  // Decrease by 1 for next move if needed
      stepperY.run();  // Start moving the stepper
      Serial.println("backward_y");
  }
  stepperY.setCurrentPosition(0);
}
void servoX_CW(){
  for (pos = 90; pos >= 0; pos -= 1) { // goes from 90 degrees to 0 degrees
    servoX.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}
void servoX_ACW(){
  for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 90 degrees
    // in steps of 1 degree
    servoX.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}
void servoY_CW(){
  for (pos = 35; pos >= 5; pos -= 1) { // goes from 90 degrees to 0 degrees
    servoY.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}
void servoY_ACW(){
  for (pos = 5; pos <= 35; pos += 1) { // goes from 0 degrees to 90 degrees
    // in steps of 1 degree
    servoY.write(pos);              // tell servo to go to position in variable 'pos'
    delay(10);                       // waits 15ms for the servo to reach the position
  }
}
