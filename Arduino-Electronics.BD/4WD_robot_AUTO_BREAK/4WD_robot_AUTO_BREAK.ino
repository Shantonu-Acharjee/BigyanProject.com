/*
 * Created by Electronics.Com.BD
 * Project: Auto Breaking System
 */

#define IN_11  4    // L298N #1 in 1 motor Right Ahead
#define IN_12  5    // L298N #1 in 2 motor Right Ahead
#define IN_13  7    // L298N #1 in 3 motor Right Back
#define IN_14  6    // L298N #1 in 4 motor Right Back
#define buzzer 8
 

void setup() {  

  
   pinMode(buzzer, OUTPUT);
   pinMode(IN_11, OUTPUT);
   pinMode(IN_12, OUTPUT);
   pinMode(IN_13, OUTPUT);
   pinMode(IN_14, OUTPUT);
   digitalWrite(buzzer, HIGH);
   Serial.begin(9600);
} 


void goAhead(){ 
        digitalWrite(IN_11, HIGH);
        digitalWrite(IN_12, LOW);

        digitalWrite(IN_13, LOW);
        digitalWrite(IN_14, HIGH);
  }

void goBack(){ 
        digitalWrite(IN_11, LOW);
        digitalWrite(IN_12, HIGH);

        digitalWrite(IN_13, HIGH);
        digitalWrite(IN_14, LOW);
  }

void goLeft(){ 
        digitalWrite(IN_11, LOW);
        digitalWrite(IN_12, HIGH);

        digitalWrite(IN_13, LOW);
        digitalWrite(IN_14, HIGH);
  }

void goRight(){    
        digitalWrite(IN_11, HIGH);
        digitalWrite(IN_12, LOW);

        digitalWrite(IN_13, HIGH);
        digitalWrite(IN_14, LOW);   
  }


void stopRobot(){  

        digitalWrite(IN_11, LOW);
        digitalWrite(IN_12, LOW);

        digitalWrite(IN_13, LOW);
        digitalWrite(IN_14, LOW);
  
  }

void distanceAhead(){     
  
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  timeEcho = pulseIn(ECHO, HIGH);
  lookAhead = timeEcho/58;

  }


void loop(){
  distanceAhead();
  Serial.println(lookAhead);
  if(lookAhead == 0){
    lookAhead = 50;
  }
  if(lookAhead < 40){
    digitalWrite(buzzer, LOW);
    stopRobot();
  }
  else{
    digitalWrite(buzzer, HIGH);
    goAhead();
  }
}
