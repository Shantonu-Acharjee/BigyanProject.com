/*
 * Created by Electronics.Com.BD
 * Project: obstacle avoidance
 */
#include <AltSoftSerial.h>
#include <ServoTimer2.h>
AltSoftSerial bluetooth(8, 9);
#define TRIG 16
#define ECHO 17
#define S_L  14
#define S_M  15
#define S_R  18
#define S_S  19
#define IN_11  2    // L298N #1 in 1 motor Right Ahead
#define IN_12  4    // L298N #1 in 2 motor Right Ahead
#define IN_13  11    // L298N #1 in 3 motor Right Back
#define IN_14  10    // L298N #1 in 4 motor Right Back
#define sw_mood  12
ServoTimer2 servo;
int angle = 90;    
int timeEcho;  
const int motorAspeed  = 5;
const int motorBspeed  = 6;
int vSpeed = 150; 
char dataReceived = 'x';   
int flag = 1;     
void setup() {     
  servo.attach(3);
  pinMode(TRIG, OUTPUT); 
  pinMode(ECHO, INPUT); 
  pinMode(S_L, INPUT); 
  pinMode(S_M, INPUT); 
  pinMode(S_R, INPUT); 
  pinMode(S_S, INPUT); 
  pinMode(sw_mood, INPUT_PULLUP); 
  pinMode(IN_11, OUTPUT);
  pinMode(IN_12, OUTPUT);
  pinMode(IN_13, OUTPUT);
  pinMode(IN_14, OUTPUT);
  Serial.begin(9600);
  bluetooth.begin(9600);
  analogWrite (motorAspeed, vSpeed);
  analogWrite (motorBspeed, vSpeed);
  delay(15000);  
  servo.write(1500);
  Serial.println("AT"); //If everything is Okay it will show "OK" on the serial monitor
  updateSerial();
  Serial.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  Serial.println("AT+CNMI=2,2,0,0,0"); //Configure the SIM800L on how to manage the Received SMS... Check the SIM800L AT commands manual
  updateSerial();
} 
void loop(){
  if(digitalRead(sw_mood)){
    //Serial.print("ON");
    if(digitalRead(S_L) == HIGH && digitalRead(S_R) == LOW){
      //Serial.print("RIGHT");
      goLeft();
      delay(100);
    }
    else if(digitalRead(S_L) == LOW && digitalRead(S_R) == HIGH){
      //Serial.print("LEFT");
      goRight();
      delay(100);
    }
    else if(digitalRead(S_L) == HIGH && digitalRead(S_R) == HIGH){
      //Serial.print("STOP");
      stopRobot();
    }
    else if(digitalRead(S_S) == HIGH){
      stopRobot();
      //Serial.print("TURN");
      delay(20000);
      goAhead();
      delay(100);
    }
    else{
      //Serial.print("AHEAD");
      goAhead();
    }
    //Serial.println();
  }
  else{
    //Serial.print("OFF");
    if (bluetooth.available()) 
    {
      dataReceived=bluetooth.read();
      //Serial.println(dataReceived);
    }
    if(dataReceived=='x')
    {
      flag = 1;
    }
    if(dataReceived=='X')
    {
      flag = 0;
    }
    
    if(flag==1)
    {
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
        goRight();
      }
      if(dataReceived=='L')
      {
        goLeft();
      }
    }
    if(flag==0)
    {
      int distanceRight = 0;
      int distanceLeft = 0;
      int distance = distanceAhead();
      if (distance == 0) {
        distance = 20;
      }
      if (distance <= 20){
        stopRobot();
        delay(300);
        goBack();
        delay(400);
        stopRobot();
        send_sms();
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
      distance = distanceAhead();
    }
    if(dataReceived=='0')
    {
      vSpeed=0;
    }
    if(dataReceived=='1')
    {
      vSpeed=30;
    }
    if(dataReceived=='2')
    {
      vSpeed=60;
    }
    if(dataReceived=='3')
    {
      vSpeed=90;
    }
    if(dataReceived=='4')
    {
      vSpeed=120;
    }
    if(dataReceived=='5')
    {
      vSpeed=150;
    }
    if(dataReceived=='6')
    {
      vSpeed=180;
    }
    if(dataReceived=='7')
    {
      vSpeed=210;
    }
    if(dataReceived=='8')
    {
      vSpeed=240;
    }      
    if(dataReceived=='9')
    {
      vSpeed=255;
    }
  }
}

void goAhead(){ 
  analogWrite (motorAspeed, vSpeed);
  analogWrite (motorBspeed, vSpeed);
  digitalWrite(IN_11, LOW);
  digitalWrite(IN_12, HIGH);
  digitalWrite(IN_13, HIGH);
  digitalWrite(IN_14, LOW);
}

void goBack(){ 
  analogWrite (motorAspeed, vSpeed);
  analogWrite (motorBspeed, vSpeed);
  digitalWrite(IN_11, HIGH);
  digitalWrite(IN_12, LOW);
  digitalWrite(IN_13, LOW);
  digitalWrite(IN_14, HIGH);
}

void goLeft(){
  analogWrite (motorAspeed, vSpeed);
  analogWrite (motorBspeed, vSpeed);
  digitalWrite(IN_11, HIGH);
  digitalWrite(IN_12, LOW);
  digitalWrite(IN_13, HIGH);
  digitalWrite(IN_14, LOW);
}

void goRight(){
  analogWrite (motorAspeed, vSpeed);
  analogWrite (motorBspeed, vSpeed);
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
  servo.write(1500);
  delay(500);
}

void lookRight(){  
  servo.write(850);
  delay(500);
}

void lookLeft(){
  servo.write(2200);
  delay(500);
}

void send_sms()
{  
  Serial.println("AT");
  updateSerial();
  Serial.println("AT+CMGF=1");
  updateSerial();
  Serial.println("AT+CMGS=\"+8801571292664\"");
  updateSerial();
  Serial.print("There is an Object Detected! Trying to find a path.");
  updateSerial();
  Serial.write(26);
}

void updateSerial()
{
  delay(500);
}
