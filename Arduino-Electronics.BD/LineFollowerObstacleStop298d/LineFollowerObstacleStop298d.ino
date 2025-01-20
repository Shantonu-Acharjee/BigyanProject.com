#include<NewPing.h>
int ENA = 6;  //ENA connected to digital pin 3
int ENB = 11;  //ENB connected to digital pin 9
int MOTOR_A1 = 8; // MOTOR_A1 connected to digital pin 4
int MOTOR_A2 = 7; // MOTOR_A2 connected to digital pin 5
int MOTOR_B1 = 9; // MOTOR_B1 connected to digital pin 6
int MOTOR_B2 = 10; // MOTOR_B2 connected to digital pin 7
int RIGHT = 3; // RIGHT sensor connected to analog pin A0
int LEFT = 2;  // LEFT sensor connected to analog pin A1
int buzzer = 4;
int led = 5;
#define TRIG A1 // TRIG PIN connected to analog pin A2
#define ECHO A0 // ECHO PIN connected to analog pin A3
#define MAX_DISTANCE 200 // Define Maximum Distance
int ENAV = 100;
int ENBV = 100;
int distance = 100;
NewPing sonar(TRIG, ECHO, MAX_DISTANCE); 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);  
  pinMode(ENA, OUTPUT); // initialize ENA pin as an output
  pinMode(ENB, OUTPUT); // initialize ENB pin as an output
  pinMode(MOTOR_A1, OUTPUT); // initialize MOTOR_A1 pin as an output
  pinMode(MOTOR_A2, OUTPUT); // initialize MOTOR_A2 pin as an output
  pinMode(MOTOR_B1, OUTPUT); // initialize MOTOR_B1 pin as an output
  pinMode(MOTOR_B2, OUTPUT); // initialize MOTOR_B2 pin as an output
  pinMode(RIGHT, INPUT); // initialize RIGHT pin as an input
  pinMode(LEFT, INPUT);  // initialize LEFT pin as an input
  pinMode(buzzer, OUTPUT);
  pinMode(led, OUTPUT);
  digitalWrite(buzzer, LOW);
  digitalWrite(led, LOW);
}

void loop() {
  distance = readPing();
  if (distance > 20){
    if (!digitalRead(RIGHT) && !digitalRead(LEFT)) {
      moveForward();
    }else if (!digitalRead(RIGHT) && digitalRead(LEFT)) {
      turnLeft();
    }else if (digitalRead(RIGHT) && !digitalRead(LEFT)) {
      turnRight(); 
    }else if (digitalRead(RIGHT) && digitalRead(LEFT)) { 
      //Stop();
      //delay(60000);
    }
    digitalWrite(buzzer, LOW);
    digitalWrite(led, LOW);
  }
  else{ 
    digitalWrite(buzzer, HIGH);
    digitalWrite(led, HIGH);
    Stop();
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
  digitalWrite(MOTOR_A1, HIGH);
  digitalWrite(MOTOR_A2, LOW);
  digitalWrite(MOTOR_B1, HIGH);
  digitalWrite(MOTOR_B2, LOW);
}
void moveBackward() {
  analogWrite(ENA, ENAV);
  analogWrite(ENB, ENBV);
  digitalWrite(MOTOR_A1, LOW);
  digitalWrite(MOTOR_A2, HIGH);
  digitalWrite(MOTOR_B1, LOW);
  digitalWrite(MOTOR_B2, HIGH);
}

int readPing(){
  //delay(30);
  int cm = sonar.ping_cm();
  if (cm==0){
    cm=250;
  }
  return cm;
}
