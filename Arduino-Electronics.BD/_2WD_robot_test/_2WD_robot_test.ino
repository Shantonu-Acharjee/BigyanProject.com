/*
 * Created by Electronics.Com.BD
 * Project: Line following robot with obstacle avoidance
 */
#include <Servo.h>

#define TRIG 8
#define ECHO 9

#define IN_11  7    // L298N #1 in 1 motor Right Ahead
#define IN_12  8    // L298N #1 in 2 motor Right Ahead
#define IN_13  9    // L298N #1 in 3 motor Right Back
#define IN_14  10    // L298N #1 in 4 motor Right Back
//Sensor Connection
const int left_sensor_pin =2;
const int right_sensor_pin =3;
//Switch for changing mood
int line_follow_mode = A0;
int obstacle_avoiding_mode = A1;
Servo servo;

int angle = 90;    
int lookLeft = 0; 
int lookRight = 0; 
int lookAhead = 0; 
int timeEcho;  
const int motorAspeed  = 6;
const int motorBspeed  =11;
int vSpeed = 200;                      
int turn_speed = 255;       // 0 - 255  max
int turn_delay = 10;
int left_sensor_state;
  int right_sensor_state;
void setup() {  
   
   servo.attach(5);
  
   pinMode(TRIG, OUTPUT); 
   pinMode(ECHO, INPUT); 
  
   pinMode(IN_11, OUTPUT);
   pinMode(IN_12, OUTPUT);
   pinMode(IN_13, OUTPUT);
   pinMode(IN_14, OUTPUT);
   pinMode(left_sensor_pin, INPUT);
   pinMode(right_sensor_pin, INPUT);


   pinMode(line_follow_mode, INPUT);
   pinMode(obstacle_avoiding_mode, INPUT);
   Serial.begin(9600);
   analogWrite (motorAspeed, vSpeed);
   analogWrite (motorBspeed, vSpeed);
   delay(1000);
   servo.write(90);
} 


void goAhead(){ 
        digitalWrite(IN_11, HIGH);
        digitalWrite(IN_12, LOW);

        digitalWrite(IN_13, HIGH);
        digitalWrite(IN_14, LOW);
  }

void goBack(){ 
        digitalWrite(IN_11, LOW);
        digitalWrite(IN_12, HIGH);

        digitalWrite(IN_13, LOW);
        digitalWrite(IN_14, HIGH);
  }

void goRight(){ 
        digitalWrite(IN_11, LOW);
        digitalWrite(IN_12, HIGH);

        digitalWrite(IN_13, HIGH);
        digitalWrite(IN_14, LOW);
  }

void goLeft(){
        digitalWrite(IN_11, HIGH);
        digitalWrite(IN_12, LOW);

        digitalWrite(IN_13, LOW);
        digitalWrite(IN_14, HIGH);       
  }


void stopRobot(){  

        digitalWrite(IN_11, LOW);
        digitalWrite(IN_12, LOW);

        digitalWrite(IN_13, LOW);
        digitalWrite(IN_14, LOW);
  
  }

void distanceLeft(){     
  
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  timeEcho = pulseIn(ECHO, HIGH);
  lookLeft = timeEcho/58;

  }

void distanceAhead(){     
  
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
    timeEcho = pulseIn(ECHO, HIGH);
  lookAhead = timeEcho/58;

  }

void distanceRight(){        
    
  digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);
    timeEcho = pulseIn(ECHO, HIGH);
    lookRight = timeEcho/58;
  
  }


void loop(){
  Serial.println("Go Ahead");
  goAhead();
  analogWrite (motorAspeed, vSpeed);
  analogWrite (motorBspeed, vSpeed);
  delay(1000);
  stopRobot();
  delay(1000);  
  Serial.println("Go Back");
  goBack();
  analogWrite (motorAspeed, vSpeed);
  analogWrite (motorBspeed, vSpeed);
  delay(1000);
  stopRobot();
  delay(1000); 
  Serial.println("Go Right"); 
  goRight();
  analogWrite (motorAspeed, vSpeed);
  analogWrite (motorBspeed, vSpeed);
  delay(1000);
  stopRobot();
  delay(1000); 
  Serial.println("Go Left"); 
  goLeft();
  analogWrite (motorAspeed, vSpeed);
  analogWrite (motorBspeed, vSpeed);
  delay(1000);
  stopRobot();
  delay(1000); 
}
