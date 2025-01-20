#include <SoftwareSerial.h>
int MotorA_IN1=7;
int MotorA_IN2=8;
int MotorB_IN3=9;
int MotorB_IN4=10;
int MotorA_PWM=5;
int MotorB_PWM=6;
int spd=150;
 
char sms;
SoftwareSerial BT(12, 11);
void setup() 
{
  Serial.begin(9600);
  BT.begin(9600);
  pinMode(MotorA_IN1,OUTPUT);
  pinMode(MotorA_IN2,OUTPUT);
  pinMode(MotorB_IN3,OUTPUT);
  pinMode(MotorB_IN4,OUTPUT);
  pinMode(MotorA_PWM,OUTPUT);
  pinMode(MotorB_PWM,OUTPUT);
  analogWrite(MotorA_PWM,spd);
  analogWrite(MotorB_PWM,spd);
}
  
 
 
void loop() 
{  
  Serial.print(" SMS=");
  Serial.print(sms);
  Serial.println();
  if (BT.available()) 
  {
    sms=BT.read();
  }
  analogWrite(MotorA_PWM,spd);
  analogWrite(MotorB_PWM,spd);
  
  if(sms=='S')
  {
    RobotStop();
  }
  if(sms=='F')
  {
    RobotForward();
  }
  if(sms=='B')
  {
    RobotBackward();
  }
  if(sms=='R')
  {
    RobotRight();
  }
  if(sms=='L')
  {
    RobotLeft();
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
void RobotForward()
{         
    digitalWrite(MotorA_IN1, LOW);
    digitalWrite(MotorA_IN2, HIGH);
    digitalWrite(MotorB_IN3, LOW);
    digitalWrite(MotorB_IN4, HIGH);
}
 
void RobotBackward()
{              
    digitalWrite(MotorA_IN1, HIGH);
    digitalWrite(MotorA_IN2, LOW);
    digitalWrite(MotorB_IN3, HIGH);
    digitalWrite(MotorB_IN4, LOW);
}
 
void RobotLeft()
{
    digitalWrite(MotorA_IN1, LOW);
    digitalWrite(MotorA_IN2, HIGH);
    digitalWrite(MotorB_IN3, HIGH);
    digitalWrite(MotorB_IN4, LOW);
}
 
void RobotRight()
{
    digitalWrite(MotorA_IN1, HIGH);
    digitalWrite(MotorA_IN2, LOW);
    digitalWrite(MotorB_IN3, LOW);
    digitalWrite(MotorB_IN4, HIGH);
} 
 
void RobotStop()
{
    digitalWrite(MotorA_IN1, LOW);
    digitalWrite(MotorA_IN2, LOW);
    digitalWrite(MotorB_IN3, LOW);
    digitalWrite(MotorB_IN4, LOW);
}
