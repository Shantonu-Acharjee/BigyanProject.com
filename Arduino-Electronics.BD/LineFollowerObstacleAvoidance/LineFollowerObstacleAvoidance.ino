#include<NewPing.h>

int ENA = 11;  //ENA connected to digital pin 3
int ENB = 6;  //ENB connected to digital pin 9
int MOTOR_A1 = 10; // MOTOR_A1 connected to digital pin 4
int MOTOR_A2 = 9; // MOTOR_A2 connected to digital pin 5
int MOTOR_B1 = 8; // MOTOR_B1 connected to digital pin 6
int MOTOR_B2 = 7; // MOTOR_B2 connected to digital pin 7

int RIGHT = 3; // RIGHT sensor connected to analog pin A0
int LEFT = 2;  // LEFT sensor connected to analog pin A1

#define TRIG A2 // TRIG PIN connected to analog pin A2
#define ECHO A3 // ECHO PIN connected to analog pin A3
#define MAX_DISTANCE 200 // Define Maximum Distance
int ENAV = 100;
int ENBV = 130;
int timeEcho;
NewPing sonar(TRIG, ECHO, MAX_DISTANCE); 

void setup() {
  Serial.begin(9600);  
  pinMode(ENA, OUTPUT); // initialize ENA pin as an output
  pinMode(ENB, OUTPUT); // initialize ENB pin as an output
  pinMode(MOTOR_A1, OUTPUT); // initialize MOTOR_A1 pin as an output
  pinMode(MOTOR_A2, OUTPUT); // initialize MOTOR_A2 pin as an output
  pinMode(MOTOR_B1, OUTPUT); // initialize MOTOR_B1 pin as an output
  pinMode(MOTOR_B2, OUTPUT); // initialize MOTOR_B2 pin as an output
  pinMode(RIGHT, INPUT); // initialize RIGHT pin as an input
  pinMode(LEFT, INPUT);  // initialize LEFT pin as an input
}

void loop() {
int distance = sonar.ping_cm();
Serial.println(distance);
if (distance == 0) {
  distance = 30;
}
if(distance <=15) {
  Serial.println("dis");
  Stop();
  delay(100);
  analogWrite(ENA, ENAV);
  analogWrite(ENB, ENBV);
  digitalWrite(MOTOR_A1, LOW);
  digitalWrite(MOTOR_A2, HIGH);
  digitalWrite(MOTOR_B1, LOW);
  digitalWrite(MOTOR_B2, LOW);
  delay(850);
  Stop();
  delay(200);
  analogWrite(ENA, ENAV);
  analogWrite(ENB, ENBV);
  digitalWrite(MOTOR_A1, LOW);
  digitalWrite(MOTOR_A2, HIGH);
  digitalWrite(MOTOR_B1, LOW);
  digitalWrite(MOTOR_B2, HIGH);
  delay(800);
  Stop();
  delay(200);
  analogWrite(ENA, ENAV);
  analogWrite(ENB, ENBV);
  digitalWrite(MOTOR_A1, LOW);
  digitalWrite(MOTOR_A2, LOW);
  digitalWrite(MOTOR_B1, LOW);
  digitalWrite(MOTOR_B2, HIGH);
  delay(800);
  Stop();
  delay(200);
  analogWrite(ENA, ENAV);
  analogWrite(ENB, ENBV);
  digitalWrite(MOTOR_A1, LOW);
  digitalWrite(MOTOR_A2, HIGH);
  digitalWrite(MOTOR_B1, LOW);
  digitalWrite(MOTOR_B2, HIGH);
  delay(800);
  Stop();
  delay(200);
  analogWrite(ENA, ENAV);
  analogWrite(ENB, ENBV);
  digitalWrite(MOTOR_A1, LOW);
  digitalWrite(MOTOR_A2, LOW);
  digitalWrite(MOTOR_B1, LOW);
  digitalWrite(MOTOR_B2, HIGH);
  delay(700);
}
if (!digitalRead(RIGHT) && !digitalRead(LEFT)) {
  //Serial.println("forword");
  moveForward();
}else if (!digitalRead(RIGHT) && digitalRead(LEFT)) {
  //Serial.println("left");
  turnLeft();
}else if (digitalRead(RIGHT) && !digitalRead(LEFT)) {
  //Serial.println("right");
  turnRight();  
}else if (digitalRead(RIGHT) && digitalRead(LEFT)) { 
  //Serial.println("back");
  turnRight();
  delay(300);
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
  digitalWrite(MOTOR_A1, LOW);
  digitalWrite(MOTOR_A2, HIGH);
  digitalWrite(MOTOR_B1, HIGH);
  digitalWrite(MOTOR_B2, LOW);
  
}

void turnLeft() {
  analogWrite(ENA, ENAV);
  analogWrite(ENB, ENBV);
  digitalWrite(MOTOR_A1, HIGH);
  digitalWrite(MOTOR_A2, LOW);
  digitalWrite(MOTOR_B1, LOW);
  digitalWrite(MOTOR_B2, HIGH);
}

void moveForward() {
  analogWrite(ENA, ENAV);
  analogWrite(ENB, ENBV);
  digitalWrite(MOTOR_A1, LOW);
  digitalWrite(MOTOR_A2, HIGH);
  digitalWrite(MOTOR_B1, LOW);
  digitalWrite(MOTOR_B2, HIGH);
}
