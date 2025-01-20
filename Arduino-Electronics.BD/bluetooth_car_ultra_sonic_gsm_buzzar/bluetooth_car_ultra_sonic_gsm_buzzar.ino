#include<NewPing.h>
#include <SoftwareSerial.h>
int MotorA_IN1=2;
int MotorA_IN2=3;
int MotorB_IN3=4;
int MotorB_IN4=5;
#define TRIG 8 // TRIG PIN connected to analog pin A2
#define ECHO 9 // ECHO PIN connected to analog pin A3
#define MAX_DISTANCE 200 // Define Maximum Distance
int buzzer = 12;
int timeEcho;
int flag = 0;
char sms;
String voice;
String message = "";   // A String for storing the message
NewPing sonar(TRIG, ECHO, MAX_DISTANCE); 
SoftwareSerial SIM800(11, 10);
SoftwareSerial BT(6, 7);
void setup() 
{
  Serial.begin(9600);
  pinMode(TRIG, OUTPUT); 
  pinMode(ECHO, INPUT);
  pinMode(MotorA_IN1,OUTPUT);
  pinMode(MotorA_IN2,OUTPUT);
  pinMode(MotorB_IN3,OUTPUT);
  pinMode(MotorB_IN4,OUTPUT);  
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);
  pinMode(13, OUTPUT);
  digitalWrite(13,HIGH); 
  BT.begin(9600); // baudrate for BT shield
  SIM800.begin(9600); // baudrate for GSM shield
  // set SMS mode to text mode
  SIM800.print("AT+CMGF=1\r");  
  delay(100);
  
  // set gsm module to tp show the output on serial out
  SIM800.print("AT+CNMI=2,2,0,0,0\r"); 
}
  
 
 
void loop() 
{    
    BT.listen();
    while(BT.available()>0)
    {
      delay(10);
     char c=BT.read();
     if(c=='#')
     
      {
        break;
      }
      voice+=c;
    }
    
    if(voice.length()>0)
      {
        Serial.println(voice);
        //voice="";
        //RobotStop();
      }
    if(voice=="stop")
    {
      RobotStop();
    }
    else if(voice=="go")
    {
      int distance = readPing();
      Serial.println(distance);
      if (distance <= 20 && flag == 0){
          digitalWrite(buzzer, HIGH);
          RobotStop();
          message = "Obstacle Detected";
          send_message(message);
          delay(2000);
          digitalWrite(buzzer, LOW); 
          flag = 1;
          voice="";
      }
      else if (distance > 20 && flag == 1){
          flag = 0;  
      }
      else if(flag == 0)
      {
        RobotForward();
        //delay(2000);
      }      
    }
    else if(voice=="back")
    {
      RobotBackward();
      delay(500);
      voice="";
       RobotStop();
    }
    else if(voice=="right")
    {
      RobotRight();
      delay(500);
      voice="";
       RobotStop();
    }
    else if(voice=="left")
    {
      RobotLeft();
      delay(500);
      voice="";
       RobotStop();
    }
    else{
      //if(voice.length()>0)
      //{
        //Serial.println(voice);
       voice="";
       RobotStop();
      //}
    }
}
void RobotForward()
{            
    digitalWrite(MotorA_IN1, HIGH);
    digitalWrite(MotorA_IN2, LOW);
    digitalWrite(MotorB_IN3, HIGH);
    digitalWrite(MotorB_IN4, LOW);
}
 
void RobotBackward()
{           
    digitalWrite(MotorA_IN1, LOW);
    digitalWrite(MotorA_IN2, HIGH);
    digitalWrite(MotorB_IN3, LOW);
    digitalWrite(MotorB_IN4, HIGH);
}
 
void RobotLeft()
{
    digitalWrite(MotorA_IN1, LOW);
    digitalWrite(MotorA_IN2, LOW);
    digitalWrite(MotorB_IN3, HIGH);
    digitalWrite(MotorB_IN4, LOW);
}
 
void RobotRight()
{
    digitalWrite(MotorA_IN1, HIGH);
    digitalWrite(MotorA_IN2, LOW);
    digitalWrite(MotorB_IN3, LOW);
    digitalWrite(MotorB_IN4, LOW);
} 
 
void RobotStop()
{
    digitalWrite(MotorA_IN1, LOW);
    digitalWrite(MotorA_IN2, LOW);
    digitalWrite(MotorB_IN3, LOW);
    digitalWrite(MotorB_IN4, LOW);
}

int readPing(){
  delay(30);
  int cm = sonar.ping_cm();
  if (cm==0){
    cm=250;
  }
  return cm;
}

void send_message(String message)
{
  SIM800.listen();
  SIM800.println("AT+CMGF=1");    //Set the GSM Module in Text Mode
  delay(100);  
  SIM800.println("AT+CMGS=\"+8801739188483\""); // Replace it with your mobile number
  delay(100);
  SIM800.println(message);   // The SMS text you want to send
  delay(100);
  SIM800.println((char)26);  // ASCII code of CTRL+Z
  delay(100);
  SIM800.println();
  delay(1000);  
}
