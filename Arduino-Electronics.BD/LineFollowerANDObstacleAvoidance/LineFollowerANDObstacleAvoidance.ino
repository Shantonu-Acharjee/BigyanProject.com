#include<NewPing.h>
#include <Servo.h>
int ENA = 6;  //ENA connected to digital pin 3
int ENB = 11;  //ENB connected to digital pin 9
int MOTOR_A1 = 7; // MOTOR_A1 connected to digital pin 4
int MOTOR_A2 = 8; // MOTOR_A2 connected to digital pin 5
int MOTOR_B1 = 9; // MOTOR_B1 connected to digital pin 6
int MOTOR_B2 = 10; // MOTOR_B2 connected to digital pin 7
Servo servo;
int RIGHT = 2; // RIGHT sensor connected to analog pin A0
int LEFT = 3;  // LEFT sensor connected to analog pin A1
int button = 4;
#define TRIG A1 // TRIG PIN connected to analog pin A2
#define ECHO A0 // ECHO PIN connected to analog pin A3
#define MAX_DISTANCE 200 // Define Maximum Distance
int ENAV = 90;
int ENBV = 90;
int distance = 100;
NewPing sonar(TRIG, ECHO, MAX_DISTANCE); 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);  
  servo.attach(5);
  pinMode(ENA, OUTPUT); // initialize ENA pin as an output
  pinMode(ENB, OUTPUT); // initialize ENB pin as an output
  pinMode(MOTOR_A1, OUTPUT); // initialize MOTOR_A1 pin as an output
  pinMode(MOTOR_A2, OUTPUT); // initialize MOTOR_A2 pin as an output
  pinMode(MOTOR_B1, OUTPUT); // initialize MOTOR_B1 pin as an output
  pinMode(MOTOR_B2, OUTPUT); // initialize MOTOR_B2 pin as an output
  pinMode(RIGHT, INPUT); // initialize RIGHT pin as an input
  pinMode(LEFT, INPUT);  // initialize LEFT pin as an input
  pinMode(button, INPUT_PULLUP);
  servo.write(90);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(button)){
    Serial.println("high");
    if (!digitalRead(RIGHT) && !digitalRead(LEFT)) {
      moveForward();
      //delay(10); 
      //Stop();
      //delay(10); 
    }else if (!digitalRead(RIGHT) && digitalRead(LEFT)) {
      turnLeft();
      delay(200);
    }else if (digitalRead(RIGHT) && !digitalRead(LEFT)) {
      turnRight(); 
      delay(200); 
    }else if (digitalRead(RIGHT) && digitalRead(LEFT)) { 
      Stop();
      delay(300);
    }
  }
  else{
    Serial.println("low");
  int distanceRight = 0;
  int distanceLeft = 0;
  delay(50);

  if (distance <= 30){
    Stop();
    delay(300);
    moveBackward();
    delay(400);
    Stop();
    delay(300);
    distanceRight = lookRight();
    delay(300);
    distanceLeft = lookLeft();
    delay(300);

    if (distance >= distanceLeft){
      turnRight();
      delay(800);
      Stop();
    }
    else{
      turnLeft();
      delay(800);
      Stop();
    }
  }
  else{
    moveForward(); 
  }
    distance = readPing();
  }

}
void Stop() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(MOTOR_A1, LOW);
  digitalWrite(MOTOR_A2, LOW);
  digitalWrite(MOTOR_B1, LOW);
  digitalWrite(MOTOR_B2, LOW);
}

void turnRight() {
  analogWrite(ENA, ENAV);
  analogWrite(ENB, ENBV);
  digitalWrite(MOTOR_A1, HIGH);
  digitalWrite(MOTOR_A2, LOW);
  digitalWrite(MOTOR_B1, HIGH);
  digitalWrite(MOTOR_B2, LOW);
  
}

void turnLeft() {
  analogWrite(ENA, ENAV);
  analogWrite(ENB, ENBV);
  digitalWrite(MOTOR_A1, LOW);
  digitalWrite(MOTOR_A2, HIGH);
  digitalWrite(MOTOR_B1, LOW);
  digitalWrite(MOTOR_B2, HIGH);
}

void moveForward() {
  analogWrite(ENA, ENAV);
  analogWrite(ENB, ENBV);
  digitalWrite(MOTOR_A1, LOW);
  digitalWrite(MOTOR_A2, HIGH);
  digitalWrite(MOTOR_B1, HIGH);
  digitalWrite(MOTOR_B2, LOW);
}
void moveBackward() {
  analogWrite(ENA, ENAV);
  analogWrite(ENB, ENBV);
  digitalWrite(MOTOR_A1, HIGH);
  digitalWrite(MOTOR_A2, LOW);
  digitalWrite(MOTOR_B1, LOW);
  digitalWrite(MOTOR_B2, HIGH);
}

int lookRight(){  
  servo.write(10);
  delay(500);
  int distance = readPing();
  delay(100);
  servo.write(90);
  return distance;
}

int lookLeft(){
  servo.write(170);
  delay(500);
  int distance = readPing();
  delay(100);
  servo.write(90);
  return distance;
  delay(100);
}

int readPing(){
  delay(70);
  int cm = sonar.ping_cm();
  if (cm==0){
    cm=250;
  }
  return cm;
}
