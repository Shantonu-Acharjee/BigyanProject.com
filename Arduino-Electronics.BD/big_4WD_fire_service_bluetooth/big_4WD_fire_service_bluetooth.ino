#include <SoftwareSerial.h>
SoftwareSerial mySerial(9, 8); // RX, TX
#include <PWMServo.h>
#define temp A5

#include "BTS7960.h"
#define TRIG 15
#define ECHO 14
const uint8_t EN1 = 4;
const uint8_t L_PWM1 = 5;
const uint8_t R_PWM1 = 6;
const uint8_t EN2 = 12;
const uint8_t L_PWM2 = 11;
const uint8_t R_PWM2 = 3;

BTS7960 motorController1(EN1, L_PWM1, R_PWM1);
BTS7960 motorController2(EN2, L_PWM2, R_PWM2);

char sms;
PWMServo myservo1;

int speed = 180;
int pos = 0;
int lookAhead = 0; 
int timeEcho;  
int smsflag = 0;  
const int fire_sensor_pin1 =16;
const int fire_sensor_pin2 =17;
const int fire_sensor_pin3 =18;
const int servo_pin1 =10;
const int relay_pin =2;
const int buz_pin =7;
void setup() {  
   Serial.begin(9600);  
   mySerial.begin(9600);  
   pinMode(TRIG, OUTPUT); 
   pinMode(ECHO, INPUT);
   pinMode(fire_sensor_pin1, INPUT);
   pinMode(fire_sensor_pin2, INPUT);
   pinMode(fire_sensor_pin3, INPUT);
   pinMode(buz_pin, OUTPUT);
   pinMode(relay_pin, OUTPUT);
   digitalWrite(relay_pin, HIGH);
   digitalWrite(buz_pin, HIGH);
   myservo1.attach(servo_pin1);
   delay(1000);
   myservo1.write(90);
   mySerial.println("AT"); //If everything is Okay it will show "OK" on the serial monitor
   updateSerial();
   mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
   updateSerial();
   mySerial.println("AT+CNMI=2,2,0,0,0"); //Configure the SIM800L on how to manage the Received SMS... Check the SIM800L AT commands manual
   updateSerial();
} 

void loop(){
  
  int flame1 = digitalRead(fire_sensor_pin1);
  int flame2 = digitalRead(fire_sensor_pin2);
  int flame3 = digitalRead(fire_sensor_pin3);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  timeEcho = pulseIn(ECHO, HIGH);
  lookAhead = timeEcho/58;
  int temp_adc_val;
  float temp_val;
  temp_adc_val = analogRead(temp);  /* Read Temperature */
  temp_val = (temp_adc_val * 4.88); /* Convert adc value to equivalent voltage */
  temp_val = (temp_val/10); /* LM35 gives output of 10mv/°C */ 
  if(temp_val >= 30)
  {
    digitalWrite(buz_pin,LOW);
  }
  else{
    digitalWrite(buz_pin,HIGH);
  }
  if(flame1 == 1 &&  flame2 == 1 && flame3 == 1){
    digitalWrite(buz_pin,HIGH);
  }
  else{
    stopRobot();
    digitalWrite(buz_pin,LOW);
    send_sms();
    spray();
    digitalWrite(buz_pin,HIGH);
  }
  if(lookAhead > 15){
    if (Serial.available()) 
    {
      sms=Serial.read();
      //Serial.print(sms);
      //Serial.print(" Dis=");
      //Serial.print(lookAhead);
      //Serial.print(" A1=");
      //Serial.print(flame1);
      //Serial.print(" A2=");
      //Serial.print(flame2);
      //Serial.print(" A3=");
      //Serial.print(flame3);
      //Serial.print(" T=");
      //Serial.print(temp_val);
      //Serial.println();
    }
    
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
      speed = 50;
    }
    if(sms=='1')
    {
      speed = 70;
    }
    if(sms=='2')
    {
      speed = 90;
    }
    if(sms=='3')
    {
      speed = 110;
    }
    if(sms=='4')
    {
      speed = 130;
    }
    if(sms=='5')
    {
      speed = 150;
    }
    if(sms=='6')
    {
      speed = 170;
    }
    if(sms=='7')
    {
      speed = 190;
    }
    if(sms=='8')
    {
      speed = 220;
    }      
    if(sms=='9')
    {
      speed = 250;
    }
  }
  else{
    goBack();
    delay(10);
    stopRobot();
  }
}

void goAhead(){ 
  motorController1.Enable();  
  motorController2.Enable();
  motorController1.TurnRight(speed);
  motorController2.TurnRight(speed); 
}

void goBack(){ 
  motorController1.Enable();  
  motorController2.Enable();
  motorController1.TurnLeft(speed);
  motorController2.TurnLeft(speed); 
}

void goRight(){ 
  motorController1.Enable();  
  motorController2.Enable();
  motorController1.TurnRight(speed);
  motorController2.TurnLeft(speed); 
}

void goLeft(){
  motorController1.Enable();  
  motorController2.Enable();
  motorController1.TurnLeft(speed);
  motorController2.TurnRight(speed);     
}


void stopRobot(){  
  motorController1.Stop();
  motorController2.Stop();
  motorController1.Disable();
  motorController2.Disable(); 
}
void spray(){
  digitalWrite(relay_pin, LOW);
  for (pos = 90; pos <= 135; pos += 1) { 
    myservo1.write(pos);             
    delay(15);                      
  }
  for (pos = 135; pos >= 45; pos -= 1) { 
    myservo1.write(pos);              
    delay(15);                      
  }
  for (pos = 45; pos <= 135 ; pos += 1) { 
    myservo1.write(pos);             
    delay(15);                      
  }
  for (pos = 135; pos >= 45; pos -= 1) { 
    myservo1.write(pos);              
    delay(15);                      
  }
  for (pos = 45; pos <= 90; pos += 1) { 
    myservo1.write(pos);             
    delay(15);                      
  }
  digitalWrite(relay_pin, HIGH);
}
void send_sms()
{  
  if(smsflag == 0){
    mySerial.println("AT");
    updateSerial();
  
    mySerial.println("AT+CMGF=1");
    updateSerial();
    mySerial.println("AT+CMGS=\"+8801841253981\""); //8801841253981
    updateSerial();
    mySerial.print("Fire detected...! take action immediately.");
    updateSerial();
    mySerial.write(26);
  }
}
void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());
  }
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());
  }
}
