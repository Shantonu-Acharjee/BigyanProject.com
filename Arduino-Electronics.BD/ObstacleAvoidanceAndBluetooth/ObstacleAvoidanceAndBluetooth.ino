#include<NewPing.h>
#include <ServoTimer2.h>
#include <SoftwareSerial.h>
//int ENA = 10;  //ENA connected to digital pin 3
//int ENB = 5;  //ENB connected to digital pin 9
int MOTOR_A1 = 9; // MOTOR_A1 connected to digital pin 4
int MOTOR_A2 = 8; // MOTOR_A2 connected to digital pin 5
int MOTOR_B1 = 6; // MOTOR_B1 connected to digital pin 6
int MOTOR_B2 = 7; // MOTOR_B2 connected to digital pin 7

#define TRIG 3 // TRIG PIN connected to analog pin A2
#define ECHO 2 // ECHO PIN connected to analog pin A3
#define MAX_DISTANCE 200 // Define Maximum Distance
int spd = 130;
char sms;
int timeEcho;
//Switch for changing mood
int obstacle_avoiding_mode = 14;
ServoTimer2 servo;
NewPing sonar(TRIG, ECHO, MAX_DISTANCE); 
SoftwareSerial bluetooth(12, 13);
void setup() {
  Serial.begin(9600);  
  pinMode(TRIG, OUTPUT); 
  pinMode(ECHO, INPUT);
  //pinMode(ENA, OUTPUT); // initialize ENA pin as an output
  //pinMode(ENB, OUTPUT); // initialize ENB pin as an output
  pinMode(MOTOR_A1, OUTPUT); // initialize MOTOR_A1 pin as an output
  pinMode(MOTOR_A2, OUTPUT); // initialize MOTOR_A2 pin as an output
  pinMode(MOTOR_B1, OUTPUT); // initialize MOTOR_B1 pin as an output
  pinMode(MOTOR_B2, OUTPUT); // initialize MOTOR_B2 pin as an output
  pinMode(obstacle_avoiding_mode, INPUT_PULLUP);
  //pinMode(4, OUTPUT);
  //digitalWrite(4,HIGH);
  bluetooth.begin(9600); 
  servo.attach(11);
  servo.write(1500);
}

void loop() {
  if(digitalRead(obstacle_avoiding_mode)){
    int distanceRight = 0;
    int distanceLeft = 0;
    int distance = sonar.ping_cm();
    if (distance == 0) {
      distance = 30;
    }
    Serial.println(distance);
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
  else{
    if (bluetooth.available()) 
    {
      sms=bluetooth.read();
    }
    
    if(sms=='S')
    {
      Stop();
    }
    if(sms=='F')
    {
      moveForward();
    }
    if(sms=='B')
    {
      moveBackward();
    }
    if(sms=='R')
    {
      turnRight();
    }
    if(sms=='L')
    {
      turnLeft();
    }
    if(sms=='0')
    {
      spd=0;
    }
    if(sms=='1')
    {
      spd=30;
    }
    if(sms=='2')
    {
      spd=60;
    }
    if(sms=='3')
    {
      spd=90;
    }
    if(sms=='4')
    {
      spd=120;
    }
    if(sms=='5')
    {
      spd=150;
    }
    if(sms=='6')
    {
      spd=180;
    }
    if(sms=='7')
    {
      spd=210;
    }
    if(sms=='8')
    {
      spd=240;
    }      
    if(sms=='9')
    {
      spd=255;
    }
  }
}
void Stop() {
  //analogWrite(ENA, 0);
  //analogWrite(ENB, 0);
  digitalWrite(MOTOR_A1, LOW);
  digitalWrite(MOTOR_A2, LOW);
  digitalWrite(MOTOR_B1, LOW);
  digitalWrite(MOTOR_B2, LOW);
}

void turnRight() {
  analogWrite(ENA, spd);
  analogWrite(ENB, spd);
  digitalWrite(MOTOR_A1, HIGH);
  digitalWrite(MOTOR_A2, LOW);
  digitalWrite(MOTOR_B1, HIGH);
  digitalWrite(MOTOR_B2, LOW);  
}

void turnLeft() {
  analogWrite(ENA, spd);
  analogWrite(ENB, spd);
  digitalWrite(MOTOR_A1, LOW);
  digitalWrite(MOTOR_A2, HIGH);
  digitalWrite(MOTOR_B1, LOW);
  digitalWrite(MOTOR_B2, HIGH);
}

void moveForward() {
  analogWrite(ENA, spd);
  analogWrite(ENB, spd);
  digitalWrite(MOTOR_A1, HIGH);
  digitalWrite(MOTOR_A2, LOW);
  digitalWrite(MOTOR_B1, LOW);
  digitalWrite(MOTOR_B2, HIGH);
}
void moveBackward() {
  analogWrite(ENA, spd);
  analogWrite(ENB, spd);
  digitalWrite(MOTOR_A1, LOW);
  digitalWrite(MOTOR_A2, HIGH);
  digitalWrite(MOTOR_B1, HIGH);
  digitalWrite(MOTOR_B2, LOW);
}
int lookRight(){  
  servo.write(850);
  delay(500);
  int distance = readPing();
  delay(100);
  servo.write(1500);
  return distance;
}

int lookLeft(){
  servo.write(2200);
  delay(500);
  int distance = readPing();
  delay(100);
  servo.write(1500);
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
