#include <Servo.h>
//#include <SoftwareSerial.h>
#define TRIG 8
#define ECHO 9
#define IN_11  4    // L298N #1 in 1 motor Right Ahead
#define IN_12  5    // L298N #1 in 2 motor Right Ahead
#define IN_13  6    // L298N #1 in 3 motor Right Back
#define IN_14  7    // L298N #1 in 4 motor Right Back
//Sensor Connection
const int switch_pin =2;
const int servo_pin =11;

char dataReceived = 'x';
Servo myservo;  
int timeEcho; 
int flag = 1; 

void setup() {  
   //Serial.begin(9600);   
   Serial.begin(9600);
   pinMode(IN_11, OUTPUT);
   pinMode(IN_12, OUTPUT);
   pinMode(IN_13, OUTPUT);
   pinMode(IN_14, OUTPUT);
   pinMode(switch_pin, INPUT_PULLUP);   
  pinMode(TRIG, OUTPUT); 
  pinMode(ECHO, INPUT);
   myservo.attach(servo_pin);
   delay(1000);
   myservo.write(90);
} 

void loop(){
  if(digitalRead(switch_pin)){
    flag = 1;
    int distanceRight = 0;
    int distanceLeft = 0;
    int distance = distanceAhead();
    //Serial.println(distance);
    if (distance == 0) {
      distance = 40;
    }
    if (distance <= 35){
      stopRobot();
      delay(300);
      goBack();
      delay(400);
      stopRobot();
      lookRight();
      distanceRight = distanceAhead();
      delay(300);
      lookAhead();
      lookLeft();
      distanceLeft = distanceAhead();
      delay(300);
      lookAhead();
  
      if (distance >= distanceLeft){
        goRight();
        delay(800);
        stopRobot();
      }
      else{
        goLeft();
        delay(800);
        stopRobot();
      }
    }
    else{
      goAhead(); 
    }
  }
  else{
    if(flag==1)
    {
      flag = 0;
      stopRobot();
    }
    if (Serial.available()) 
    {
      dataReceived=Serial.read();
      Serial.println(dataReceived);
    }
    if(dataReceived=='S')
    {
      stopRobot();
    }
    if(dataReceived=='F')
    {
      goAhead();
    }
    if(dataReceived=='B')
    {
      goBack();
    }
    if(dataReceived=='R')
    {
      dataReceived='S';
      goRight();
      delay(400);
      stopRobot();
    }
    if(dataReceived=='L')
    {
      dataReceived='S';
      goLeft();
      delay(400);
      stopRobot();
    }
  }
      
}

void goAhead(){ 
        digitalWrite(IN_11, LOW);
        digitalWrite(IN_12, HIGH);

        digitalWrite(IN_13, HIGH);
        digitalWrite(IN_14, LOW);
  }

void goBack(){ 
        digitalWrite(IN_11, HIGH);
        digitalWrite(IN_12, LOW);

        digitalWrite(IN_13, LOW);
        digitalWrite(IN_14, HIGH);
  }

void goLeft(){ 
        digitalWrite(IN_11, HIGH);
        digitalWrite(IN_12, LOW);

        digitalWrite(IN_13, HIGH);
        digitalWrite(IN_14, LOW); 
  }

void goRight(){      
        digitalWrite(IN_11, LOW);
        digitalWrite(IN_12, HIGH);

        digitalWrite(IN_13, LOW);
        digitalWrite(IN_14, HIGH);
  }


void stopRobot(){  

        digitalWrite(IN_11, LOW);
        digitalWrite(IN_12, LOW);

        digitalWrite(IN_13, LOW);
        digitalWrite(IN_14, LOW);
  
  }
int distanceAhead(){     
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  timeEcho = pulseIn(ECHO, HIGH);
  int Distance = timeEcho/58;
  return Distance;
}

void lookAhead(){
  myservo.write(90);
  delay(500);
}

void lookRight(){  
  myservo.write(45);
  delay(500);
}

void lookLeft(){
  myservo.write(135);
  delay(500);
}

