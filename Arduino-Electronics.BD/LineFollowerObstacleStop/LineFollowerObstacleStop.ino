#include<NewPing.h>
#include <MotorDriver.h>
MotorDriver m;
int RIGHT = A4; // RIGHT sensor connected to analog pin A0
int LEFT = A5;  // LEFT sensor connected to analog pin A1
int distance = 100;
int vspeed = 80;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);  
  pinMode(RIGHT, INPUT); // initialize RIGHT pin as an input
  pinMode(LEFT, INPUT);  // initialize LEFT pin as an input
}

void loop() {
    if (!digitalRead(RIGHT) && !digitalRead(LEFT)) {
      m.motor(1,FORWARD,vspeed);
      m.motor(2,FORWARD,vspeed); 
      m.motor(3,FORWARD,vspeed); 
      m.motor(4,FORWARD,vspeed);
    }else if (!digitalRead(RIGHT) && digitalRead(LEFT)) {
      m.motor(1,BACKWARD,vspeed);
      m.motor(2,BACKWARD,vspeed); 
      m.motor(3,FORWARD,vspeed); 
      m.motor(4,FORWARD,vspeed); 
      //delay(300);
    }else if (digitalRead(RIGHT) && !digitalRead(LEFT)) {
      m.motor(1,FORWARD,vspeed);
      m.motor(2,FORWARD,vspeed); 
      m.motor(3,BACKWARD,vspeed); 
      m.motor(4,BACKWARD,vspeed); 
      //delay(300); 
    }else if (digitalRead(RIGHT) && digitalRead(LEFT)) { 
      m.motor(1,BRAKE,10);
      m.motor(2,BRAKE,10); 
      m.motor(3,BRAKE,10); 
      m.motor(4,BRAKE,10);
    }
}
