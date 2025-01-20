/*
 * Created by Electronics.Com.BD
 * Project: 4WD test
 */

#define IN_11  2    // L298N #1 in 1 motor Right Ahead
#define IN_12  3    // L298N #1 in 2 motor Right Ahead
#define IN_13  4    // L298N #1 in 3 motor Right Back
#define IN_14  7    // L298N #1 in 4 motor Right Back

const int motorAspeed  = 5;
const int motorBspeed  =6;
int vSpeed = 150;
void setup() {  
   
  
   pinMode(IN_11, OUTPUT);
   pinMode(IN_12, OUTPUT);
   pinMode(IN_13, OUTPUT);
   pinMode(IN_14, OUTPUT);



   Serial.begin(9600);
   analogWrite (motorAspeed, vSpeed);
   analogWrite (motorBspeed, vSpeed);

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

void goLeft(){ 
        digitalWrite(IN_11, LOW);
        digitalWrite(IN_12, HIGH);

        digitalWrite(IN_13, HIGH);
        digitalWrite(IN_14, LOW);
  }

void goRight(){    
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


void loop(){
  //Serial.println("Go Ahead");
  //goLeft();
  /*analogWrite (motorAspeed, vSpeed);
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
  delay(1000); */
}
