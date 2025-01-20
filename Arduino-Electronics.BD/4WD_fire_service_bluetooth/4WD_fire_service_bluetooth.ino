#include <Servo.h>
//#include <SoftwareSerial.h>

#define IN_11  4    // L298N #1 in 1 motor Right Ahead
#define IN_12  5    // L298N #1 in 2 motor Right Ahead
#define IN_13  6    // L298N #1 in 3 motor Right Back
#define IN_14  7    // L298N #1 in 4 motor Right Back
//Sensor Connection
const int fire_sensor_pin =2;
const int servo_pin =11;
const int relay_pin =8;

char sms;
//SoftwareSerial BT(12, 10);
Servo myservo;

const int motorAspeed  = 3;
const int motorBspeed  = 3;
int spd = 90;
int pos = 0;

void setup() {  
   //Serial.begin(9600);   
   Serial.begin(9600);
   pinMode(IN_11, OUTPUT);
   pinMode(IN_12, OUTPUT);
   pinMode(IN_13, OUTPUT);
   pinMode(IN_14, OUTPUT);
   pinMode(relay_pin, OUTPUT);
   digitalWrite(relay_pin, LOW);
   analogWrite (motorAspeed, spd);
   analogWrite (motorBspeed, spd);
   myservo.attach(servo_pin);
   delay(1000);
   myservo.write(90);
} 

void loop(){
  int flame = digitalRead(fire_sensor_pin);
  //Serial.print(" flame=");
  //Serial.print(flame);
  //Serial.println();
  if(flame == 0)
  {    
    stopRobot();
    spray();
  }
  if (Serial.available()) 
  {
    sms=Serial.read();
    //Serial.print(sms);
    Serial.println();
  }
  
  analogWrite (motorAspeed, spd);
  analogWrite (motorBspeed, spd);
  if(sms=='S')
  {
    stopRobot();
  }
  if(sms=='F')
  {
    goAhead();
  }
  if(sms=='B')
  {
    goBack();
  }
  if(sms=='R')
  {
    goRight();
  }
  if(sms=='L')
  {
    goLeft();
  }
  if(sms=='X')
  {
    spray();
  }
  if(sms=='x')
  {
    spray();
  }
  if(sms=='0')
  {
    myservo.write(90);
  }
  if(sms=='1')
  {
    myservo.write(80);
  }
  if(sms=='2')
  {
    myservo.write(70);
  }
  if(sms=='3')
  {
    myservo.write(60);
  }
  if(sms=='4')
  {
    myservo.write(50);
  }
  if(sms=='5')
  {
    myservo.write(40);
  }
  if(sms=='6')
  {
    myservo.write(30);
  }
  if(sms=='7')
  {
    myservo.write(20);
  }
  if(sms=='8')
  {
    myservo.write(10);
  }      
  if(sms=='9')
  {
    myservo.write(0);
  }
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
void spray(){
  digitalWrite(relay_pin, HIGH);
  for (pos = 90; pos <= 135; pos += 1) { 
    myservo.write(pos);             
    delay(15);                      
  }
  for (pos = 135; pos >= 45; pos -= 1) { 
    myservo.write(pos);              
    delay(15);                      
  }
  for (pos = 45; pos <= 135 ; pos += 1) { 
    myservo.write(pos);             
    delay(15);                      
  }
  for (pos = 135; pos >= 45; pos -= 1) { 
    myservo.write(pos);              
    delay(15);                      
  }
  for (pos = 45; pos <= 90; pos += 1) { 
    myservo.write(pos);             
    delay(15);                      
  }
  digitalWrite(relay_pin, LOW);
}
