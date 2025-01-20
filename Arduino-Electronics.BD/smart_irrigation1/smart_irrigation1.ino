#include <Servo.h> // servo motor Library
Servo myservo; // declear servo class as myservo
int soil_sensor1=2; // one soil sensor attached on pin 2
int soil_sensor2=3; // other soil sensor attached on pin 3
int pump=8; // pump relay pin attached on pin 8
void setup() {
  pinMode(soil_sensor1,INPUT); //declear soil sensor pin as an input pin 
  pinMode(soil_sensor2,INPUT); //declear soil sensor pin as an input pin 
  pinMode(pump,OUTPUT); //declear pump relay pin as an output pin 
  digitalWrite(pump,HIGH); // set pump relay pin to HIGH so relay will be off when arduino start
  myservo.attach(9); // servo motor attached on pin 9
}

void loop() {
 int pos;
 if(!digitalRead(soil_sensor1) && !digitalRead(soil_sensor2)){ // if both soil sensor pins are high thats mean there is enough water so pump off
  digitalWrite(pump,HIGH);   // pump off
 }
 else if(!digitalRead(soil_sensor1) && digitalRead(soil_sensor2)){ // if first soil sensor pins are high thats mean there is not enough water so pump on
  
  digitalWrite(pump,LOW); // pump on
  for (pos = 30; pos <= 60; pos += 1) {  // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);  // tell servo to go to position in variable 'pos'
    delay(15);           // waits 15ms for the servo to reach the position
  }
  for (pos = 60; pos >= 30; pos -= 1) {  // goes from 180 degrees to 0 degrees
    myservo.write(pos);                  // tell servo to go to position in variable 'pos'
    delay(15);                           // waits 15ms for the servo to reach the position
  }  
 }
 else if(digitalRead(soil_sensor1) && !digitalRead(soil_sensor2)){ // if 2nd soil sensor pins are high thats mean there is not enough water so pump on  
  digitalWrite(pump,LOW); // pump on
  
  for (pos = 120; pos <= 150; pos += 1) {  // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);  // tell servo to go to position in variable 'pos'
    delay(15);           // waits 15ms for the servo to reach the position
  }
  for (pos = 150; pos >= 120; pos -= 1) {  // goes from 180 degrees to 0 degrees
    myservo.write(pos);                  // tell servo to go to position in variable 'pos'
    delay(15);                           // waits 15ms for the servo to reach the position
  }  
 }
 else{
  digitalWrite(pump,LOW); // pump on
  for (pos = 30; pos <= 150; pos += 1) {  // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);  // tell servo to go to position in variable 'pos'
    delay(15);           // waits 15ms for the servo to reach the position
  }
  for (pos = 150; pos >= 30; pos -= 1) {  // goes from 180 degrees to 0 degrees
    myservo.write(pos);                  // tell servo to go to position in variable 'pos'
    delay(15);                           // waits 15ms for the servo to reach the position
  }  
 }
}
