/*
   Created by Electronics.Com.BD
   Project: Line following robot with obstacle avoidance
*/

#include <SoftwareSerial.h>
#define TRIG 5
#define ECHO 4
#define IN_11  7    // L298N #1 in 1 motor Right Ahead
#define IN_12  8    // L298N #1 in 2 motor Right Ahead
#define IN_13  9    // L298N #1 in 3 motor Right Back
#define IN_14  10    // L298N #1 in 4 motor Right Back
//Sensor Connection
const int left_sensor_pin = 3;
const int right_sensor_pin = 2;
//Switch for changing mood
int line_follow_mode = 12;
int voice_mode = 17;
int timeEcho;
const int motorAspeed  = 6;
const int motorBspeed  = 11;
int spd = 90;
int lookAhead = 20;
int flag = 0;
String sms;
SoftwareSerial BT(A1, A0);
void setup() {
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(IN_11, OUTPUT);
  pinMode(IN_12, OUTPUT);
  pinMode(IN_13, OUTPUT);
  pinMode(IN_14, OUTPUT);
  pinMode(left_sensor_pin, INPUT);
  pinMode(right_sensor_pin, INPUT);
  pinMode(line_follow_mode, INPUT_PULLUP);
  pinMode(voice_mode, INPUT_PULLUP);
  Serial.begin(9600);
  BT.begin(9600);
  analogWrite (motorAspeed, spd);
  analogWrite (motorBspeed, spd);
  delay(1000);
}

void loop() {
  Serial.print("L=");
  Serial.print(digitalRead(left_sensor_pin));
  Serial.print(" R=");
  Serial.print(digitalRead(right_sensor_pin));
  Serial.print(" A=");
  Serial.print(lookAhead);
  Serial.print(" SMS=");
  Serial.print(sms);
  Serial.println();
  if (!digitalRead(line_follow_mode)) {
    spd = 90;
    if (lookAhead <= 15) {
      stopRobot();
      delay(100);
      if (flag == 0)
      {
        flag = 1;
        goRight();
        delay(700);
        goAhead();
        delay(800);
      }
      else
      {
        stopRobot();
      }
    }
    if (!digitalRead(right_sensor_pin) && !digitalRead(left_sensor_pin)) {
      distanceAhead();
      goAhead();
    } else if (!digitalRead(right_sensor_pin) && digitalRead(left_sensor_pin)) {
      goLeft();
    } else if (digitalRead(right_sensor_pin) && !digitalRead(left_sensor_pin)) {
      goRight();
    } else if (digitalRead(right_sensor_pin) && digitalRead(left_sensor_pin)) {
      stopRobot();
      if(flag != 1)
      delay(5000);
    }
  }
  else if (!digitalRead(voice_mode)) {
    sms = "";
    if (BT.available())
    {
      char c = BT.read();
      sms = c;
    }
    analogWrite(motorAspeed, 110);
    analogWrite(motorBspeed, 110);

    if (sms == "S")
    {
      stopRobot();
    }
    if (sms == "F")
    {
      goAhead();
    }
    if (sms == "B")
    {
      goBack();
    }
    if (sms == "R")
    {
      goRight();
    }
    if (sms == "L")
    {
      goLeft();
    }
    if (sms == "0")
    {
      spd = 0;
    }
    if (sms == "1")
    {
      spd = 30;
    }
    if (sms == "2")
    {
      spd = 60;
    }
    if (sms == "3")
    {
      spd = 90;
    }
    if (sms == "4")
    {
      spd = 120;
    }
    if (sms == "5")
    {
      spd = 150;
    }
    if (sms == "6")
    {
      spd = 180;
    }
    if (sms == "7")
    {
      spd = 210;
    }
    if (sms == "8")
    {
      spd = 240;
    }
    if (sms == "9")
    {
      spd = 255;
    }
    if (sms == "")
    {
      stopRobot();
    }
  }
  else {
    sms = "";
    BT.listen();
    while (BT.available() > 0)
    {
      delay(10);
      char c = BT.read();
      if (c == '#')
      {
        break;
      }
      sms += c;
    }

    analogWrite(motorAspeed, 110);
    analogWrite(motorBspeed, 110);
    if (sms == "go")
    {
      goAhead();
      delay(500);
    }
    if (sms == "back")
    {
      goBack();
      delay(500);
    }
    if (sms == "right")
    {
      goRight();
      delay(500);
    }
    if (sms == "left")
    {
      goLeft();
      delay(500);
    }
    if (sms == "")
    {
      stopRobot();
    }
  }
}

void goAhead() {
  digitalWrite(IN_11, HIGH);
  digitalWrite(IN_12, LOW);
  digitalWrite(IN_13, HIGH);
  digitalWrite(IN_14, LOW);
}

void goBack() {
  digitalWrite(IN_11, LOW);
  digitalWrite(IN_12, HIGH);
  digitalWrite(IN_13, LOW);
  digitalWrite(IN_14, HIGH);
}

void goRight() {
  digitalWrite(IN_11, HIGH);
  digitalWrite(IN_12, LOW);
  digitalWrite(IN_13, LOW);
  digitalWrite(IN_14, HIGH);
}

void goLeft() {
  digitalWrite(IN_11, LOW);
  digitalWrite(IN_12, HIGH);
  digitalWrite(IN_13, HIGH);
  digitalWrite(IN_14, LOW);
}


void stopRobot() {
  digitalWrite(IN_11, LOW);
  digitalWrite(IN_12, LOW);
  digitalWrite(IN_13, LOW);
  digitalWrite(IN_14, LOW);
}

void distanceAhead() {
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  timeEcho = pulseIn(ECHO, HIGH);
  lookAhead = timeEcho / 58;
}
