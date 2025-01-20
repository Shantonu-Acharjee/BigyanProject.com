/*
Program: Control 6 SG90s Independently Using Arduino
File: control_6_sg90s_independently_using_arduino.ino
Description: Control 6 servos independently using Arduino.
Author: Addison Sears-Collins
Website: https://automaticaddison.com
Date: June 29, 2020
*/
  
#include <Servo.h>
 
// Define the number of servos
#define SERVOS 4
 
// Define the number of states
#define STATES 7
 
// Create the servo objects.
Servo myservo[SERVOS];  
 
// Attach servos to digital pins on the Arduino
int servo_pins[SERVOS] = {5,9,10,11}; 
 
// Potential angle values of the servos in degrees
int angle_states[STATES] = {180, 150, 120, 90, 60, 30, 0};
 
void setup() {
   
  for(int i = 0; i < SERVOS; i++) {
     
    // Attach the servo to the servo object 
    myservo[i].attach(servo_pins[i]); 
 
    // Wait 500 milliseconds 
    delay(500);  
  } 
}
  
void loop() {
 
  // Move in one direction.
  for(int i = 0; i < STATES; i++) {
     
    myservo[0].write(angle_states[i]);
    delay(100);
    myservo[1].write(angle_states[i]);
    delay(100);
    myservo[2].write(angle_states[i]);
    delay(100);
    myservo[3].write(angle_states[i]);
    delay(100);
    myservo[4].write(angle_states[i]);
    delay(100);
    myservo[5].write(angle_states[i]);
    delay(100); 
  }
 
  // Move in the other direction
  for(int i = (STATES - 1); i >= 0; i--) {
     
    myservo[0].write(angle_states[i]);
    delay(100);
    myservo[1].write(angle_states[i]);
    delay(100);
    myservo[2].write(angle_states[i]);
    delay(100);
    myservo[3].write(angle_states[i]);
    delay(100);
    myservo[4].write(angle_states[i]);
    delay(100);
    myservo[5].write(angle_states[i]);
    delay(100); 
  }
}       