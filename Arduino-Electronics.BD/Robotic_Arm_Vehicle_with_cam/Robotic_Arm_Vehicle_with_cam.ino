#include <QTRSensors.h>

int aphase1 = 9;
int aphase2 = 8;
int bphase1 = 7;
int bphase2 = 5;
int aenlb = 6;
int benbl = 3;

int P;
int I;
int D;

float Kp = 0.05;
float Ki = 0.00001;
float Kd = 0.8;

int lastError = 0;

int button_calibration = A3;
int button_start = 2;

QTRSensors qtr;

const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];

void setup() {
  // put your setup code here, to run once:
  qtr.setTypeRC();
  qtr.setSensorPins((const uint8_t[]){10, 11, 12, A0, A1, A2, A4, A5}, SensorCount);
  qtr.setEmitterPin(7);

  pinMode(aphase1, OUTPUT);
  pinMode(aphase2, OUTPUT);
  pinMode(aenlb, OUTPUT);
  pinMode(bphase1, OUTPUT);
  pinMode(bphase2, OUTPUT);
  pinMode(benbl, OUTPUT);

  pinMode(button_calibration, INPUT);
  pinMode(button_start, INPUT); 
  digitalWrite(bphase2,HIGH);

  //while(digitalRead(button_calibration) == LOW) {}
  //10 seconds
  for (uint16_t i = 0; i < 400; i++)
  {
    qtr.calibrate();
  }

  //while(digitalRead(button_start) == LOW) {}
}

void loop() {
  // put your main code here, to run repeatedly:
  PID_control();
  
}

void PID_control() {
  uint16_t positionLine = qtr.readLineBlack(sensorValues);
  int error = 3500 - positionLine;

  P = error;
  I = error + I;
  D = error - lastError;
  lastError = error; 

  int motorSpeedChange = P*Kp + I*Ki + D*Kd;

  int motorSpeedA = 100 + motorSpeedChange;
  int motorSpeedB = 100 - motorSpeedChange;

  if (motorSpeedA > 125) {
    motorSpeedA = 125;
  }
  if (motorSpeedB > 125) {
    motorSpeedB = 125;
  }
  if (motorSpeedA < -75) {
    motorSpeedA = -75;
  }
  if (motorSpeedB < -75) {
    motorSpeedB = -75;
  }
  forward_movement(motorSpeedA, motorSpeedB);
}

void forward_movement(int speedB, int speedA) {
  if (speedA < 0) {
    speedA = 0 - speedA;
    digitalWrite(aphase1, HIGH);
    digitalWrite(aphase2,LOW);
  }
  else {
    digitalWrite(aphase1, LOW);
    digitalWrite(aphase2,HIGH);
  }
  if (speedB < 0) {
    speedB = 0 - speedB;
    digitalWrite(bphase1, HIGH);
    digitalWrite(bphase2,LOW);
  }
  else {
    digitalWrite(bphase1, LOW);
    digitalWrite(bphase2,HIGH);
  }
  analogWrite(aenlb, speedA);
  analogWrite(benbl, speedB);
}
