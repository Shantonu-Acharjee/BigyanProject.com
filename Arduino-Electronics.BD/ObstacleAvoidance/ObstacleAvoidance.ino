#include<NewPing.h>
//#include <Servo.h>
int MOTOR_A1 = 3; // MOTOR_A1 connected to digital pin 4
int MOTOR_A2 = 4; // MOTOR_A2 connected to digital pin 5
int MOTOR_B1 = 5; // MOTOR_B1 connected to digital pin 6
int MOTOR_B2 = 6; // MOTOR_B2 connected to digital pin 7

#define TRIG A0 // TRIG PIN connected to analog pin A2
#define ECHO A1 // ECHO PIN connected to analog pin A3
#define MAX_DISTANCE 200 // Define Maximum Distance
int spd = 130;
int timeEcho;
//Servo myservo;
NewPing sonar(TRIG, ECHO, MAX_DISTANCE); 
void setup() {
  Serial.begin(9600);  
  pinMode(TRIG, OUTPUT); 
  pinMode(ECHO, INPUT);
  pinMode(MOTOR_A1, OUTPUT); // initialize MOTOR_A1 pin as an output
  pinMode(MOTOR_A2, OUTPUT); // initialize MOTOR_A2 pin as an output
  pinMode(MOTOR_B1, OUTPUT); // initialize MOTOR_B1 pin as an output
  pinMode(MOTOR_B2, OUTPUT); // initialize MOTOR_B2 pin as an output
  //myservo.attach(11);
  //myservo.write(90);
}

void loop() {
    int distanceRight = 0;
    int distanceLeft = 0;
    int distance = readPing();
    Serial.println(distance);
    if (distance == 0) {
      distance = 30;
    }
    if (distance <= 30){
      Stop();
      delay(300);
      /*moveBackward();
      delay(400);
      Stop();
      delay(300);
      distanceRight = lookRight();
      delay(300);
      distanceLeft = lookLeft();
      delay(300);
  
      if (distance >= distanceLeft){
        turnRight();
        delay(700);
        Stop();
      }
      else{
        turnLeft();
        delay(700);
        Stop();
      }*/
    }
    else{
      moveForward(); 
    }
    distance = readPing();
}
void Stop() {
  digitalWrite(MOTOR_A1, LOW);
  digitalWrite(MOTOR_A2, LOW);
  digitalWrite(MOTOR_B1, LOW);
  digitalWrite(MOTOR_B2, LOW);
}

void turnRight() {
  digitalWrite(MOTOR_A1, LOW);
  digitalWrite(MOTOR_A2, HIGH);
  digitalWrite(MOTOR_B1, HIGH);
  digitalWrite(MOTOR_B2, LOW);  
}

void turnLeft() {
  digitalWrite(MOTOR_A1, HIGH);
  digitalWrite(MOTOR_A2, LOW);
  digitalWrite(MOTOR_B1, LOW);
  digitalWrite(MOTOR_B2, HIGH);
}

void moveForward() {
  digitalWrite(MOTOR_A1, LOW);
  digitalWrite(MOTOR_A2, HIGH);
  digitalWrite(MOTOR_B1, LOW);
  digitalWrite(MOTOR_B2, HIGH);
}
void moveBackward() {
  digitalWrite(MOTOR_A1, HIGH);
  digitalWrite(MOTOR_A2, LOW);
  digitalWrite(MOTOR_B1, HIGH);
  digitalWrite(MOTOR_B2, LOW);
}
int lookRight(){  
  //myservo.write(10);
  delay(500);
  int distance = readPing();
  delay(100);
  //myservo.write(90);
  return distance;
}

int lookLeft(){
  //myservo.write(170);
  delay(500);
  int distance = readPing();
  delay(100);
  //myservo.write(90);
  return distance;
}

int readPing(){
  delay(70);
  int cm = sonar.ping_cm();
  if (cm==0){
    cm=250;
  }
  return cm;
}
