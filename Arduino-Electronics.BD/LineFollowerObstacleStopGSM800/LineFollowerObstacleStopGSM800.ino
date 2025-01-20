#include <SoftwareSerial.h>
int ENA = 6;  //ENA connected to digital pin 3
int ENB = 11;  //ENB connected to digital pin 9
int MOTOR_A1 = 7; // MOTOR_A1 connected to digital pin 4
int MOTOR_A2 = 8; // MOTOR_A2 connected to digital pin 5
int MOTOR_B1 = 9; // MOTOR_B1 connected to digital pin 6
int MOTOR_B2 = 10; // MOTOR_B2 connected to digital pin 7
int RIGHT = 3; // RIGHT sensor connected to analog pin A0
int LEFT = 2;  // LEFT sensor connected to analog pin A1
int buzzer = 12;
#define TRIG A1 // TRIG PIN connected to analog pin A2
#define ECHO A0 // ECHO PIN connected to analog pin A3
int ENAV = 90;
int ENBV = 90;
SoftwareSerial mySerial(5, 4);
int flag = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  mySerial.begin(9600);  
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(ENA, OUTPUT); // initialize ENA pin as an output
  pinMode(ENB, OUTPUT); // initialize ENB pin as an output
  pinMode(MOTOR_A1, OUTPUT); // initialize MOTOR_A1 pin as an output
  pinMode(MOTOR_A2, OUTPUT); // initialize MOTOR_A2 pin as an output
  pinMode(MOTOR_B1, OUTPUT); // initialize MOTOR_B1 pin as an output
  pinMode(MOTOR_B2, OUTPUT); // initialize MOTOR_B2 pin as an output
  pinMode(RIGHT, INPUT); // initialize RIGHT pin as an input
  pinMode(LEFT, INPUT);  // initialize LEFT pin as an input
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);
  delay(13000);
}

void loop() {
  flag++;
  long duration, distance;
  digitalWrite(TRIG, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(TRIG, LOW);
  duration = pulseIn(ECHO, HIGH);
  distance = (duration/2) / 29.1;
  Serial.println(distance);
  if (distance > 20){
    if (!digitalRead(RIGHT) && !digitalRead(LEFT)) {
      moveForward();
    }else if (!digitalRead(RIGHT) && digitalRead(LEFT)) {
      turnLeft();
      delayMicroseconds(20);
    }else if (digitalRead(RIGHT) && !digitalRead(LEFT)) {
      turnRight(); 
      delayMicroseconds(20);
    }else if (digitalRead(RIGHT) && digitalRead(LEFT)) { 
      //Stop();
      //delay(60000);
    }
    noTone(buzzer);
  }
  else{ 
  Serial.println("object");
    Stop();
    send_sms();
    delay(50);
    send_sms1();
    for(int i=1;i<=40000;i++){
      for(int hz = 440; hz < 1000; hz+=25){
        tone(buzzer, hz, 50);
        delay(5);
      }
      // Whoop down
      for(int hz = 1000; hz > 440; hz-=25){
        tone(buzzer, hz, 50);
        delay(5);
      }
    }
  }
  if(flag == 1000){
  Serial.println("Out");
    Stop();
      for(int hz = 440; hz < 1000; hz++){
        tone(buzzer, hz, 50);
        delay(5);
      }
      for(int hz = 1000; hz > 440; hz--){
        tone(buzzer, hz, 50);
        delay(5);
    }
    flag = 0;
  }
}
void Stop() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(MOTOR_A1, LOW);
  digitalWrite(MOTOR_A2, LOW);
  digitalWrite(MOTOR_B1, LOW);
  digitalWrite(MOTOR_B2, LOW);
}

void turnRight() {
  analogWrite(ENA, ENAV);
  analogWrite(ENB, ENBV);
  digitalWrite(MOTOR_A1, HIGH);
  digitalWrite(MOTOR_A2, LOW);
  digitalWrite(MOTOR_B1, HIGH);
  digitalWrite(MOTOR_B2, LOW);
  
}

void turnLeft() {
  analogWrite(ENA, ENAV);
  analogWrite(ENB, ENBV);
  digitalWrite(MOTOR_A1, LOW);
  digitalWrite(MOTOR_A2, HIGH);
  digitalWrite(MOTOR_B1, LOW);
  digitalWrite(MOTOR_B2, HIGH);
}

void moveForward() {
  analogWrite(ENA, ENAV);
  analogWrite(ENB, ENBV);
  digitalWrite(MOTOR_A1, LOW);
  digitalWrite(MOTOR_A2, HIGH);
  digitalWrite(MOTOR_B1, HIGH);
  digitalWrite(MOTOR_B2, LOW);
}
void moveBackward() {
  analogWrite(ENA, ENAV);
  analogWrite(ENB, ENBV);
  digitalWrite(MOTOR_A1, HIGH);
  digitalWrite(MOTOR_A2, LOW);
  digitalWrite(MOTOR_B1, LOW);
  digitalWrite(MOTOR_B2, HIGH);
}

void send_sms()
{  
    mySerial.println("AT");
    updateSerial();  
    mySerial.println("AT+CMGF=1");
    updateSerial();
    mySerial.println("AT+CMGS=\"+01571116673\"");
    updateSerial();
    mySerial.print("Object detected...! take action immediately.");
    updateSerial();
    mySerial.write(26);
}
void send_sms1()
{  
    mySerial.println("AT");
    updateSerial();  
    mySerial.println("AT+CMGF=1");
    updateSerial();
    mySerial.println("AT+CMGS=\"+01991041204\"");
    updateSerial();
    mySerial.print("Object detected...! take action immediately.");
    updateSerial();
    mySerial.write(26);
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
