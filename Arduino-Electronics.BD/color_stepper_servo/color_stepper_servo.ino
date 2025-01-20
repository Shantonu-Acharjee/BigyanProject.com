#include <Servo.h>

// SENSOR
const byte S0 = 52;  
const byte S1 = 53;  
const byte S2 = 51;  
const byte S3 = 50;  
const byte sensorOut = 49;   

//MOTOR
const byte stepPinX = 17; 
const byte dirPinX = 16; 

const byte stepPinY = 15; 
const byte dirPinY = 14; 

//Limit Switch
const byte SwitchTopX = 21;
const byte SwitchTopY = 4;


const byte EnableX = 2;
const byte EnableY = 3;

//LED pins connected to Arduino
const byte redLed = 22;  
const byte greenLed = 23;  
const byte blueLed = 24;

Servo servoTop;  // create servo object to control a servo
Servo servoBottom;  // create servo object to control a servo

// Variables  
// Stores frequency read by the photodiodes
int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;

int posTop = 0;    // variable to store the servo position
int posBottom = 5;    // variable to store the servo position 
int TopX = 0;
int TopY = 0;
int Speed = 1300;

int start_flag = 0;
    
void setup()   
{  
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  digitalWrite(6,LOW);
  digitalWrite(7,LOW);
  Serial.begin(9600); 
  pinMode(S0, OUTPUT);  
  pinMode(S1, OUTPUT);  
  pinMode(S2, OUTPUT);  
  pinMode(S3, OUTPUT);  
  pinMode(sensorOut, INPUT); 
  pinMode(48, OUTPUT); 
  digitalWrite(48,HIGH);
  // Setting frequency scaling to 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);

  // Power on Color sensor
  pinMode(10, OUTPUT);
  digitalWrite(10,HIGH); 
  
  pinMode(redLed, OUTPUT);  
  pinMode(greenLed, OUTPUT);  
  pinMode(blueLed, OUTPUT);  
  pinMode(stepPinX,OUTPUT); 
  pinMode(dirPinX,OUTPUT);
  pinMode(stepPinY,OUTPUT); 
  pinMode(dirPinY,OUTPUT);
  pinMode(EnableX,OUTPUT);
  pinMode(EnableY,OUTPUT);
  digitalWrite(EnableX,HIGH);
  digitalWrite(EnableY,HIGH);
  pinMode(SwitchTopX, INPUT_PULLUP);
  pinMode(SwitchTopY, OUTPUT);  
  digitalWrite(SwitchTopY,HIGH);
    
  attachInterrupt(digitalPinToInterrupt(SwitchTopX), limittopx, RISING);
  //attachInterrupt(digitalPinToInterrupt(SwitchTopY), limittopy, RISING);
  //SERVO
  servoTop.write(0);
  servoBottom.write(0);
  servoTop.attach(7);
  servoBottom.attach(6); 
  servoTop.write(0);
  servoBottom.write(0); 
  delay(2000);
  digitalWrite(EnableY,LOW);
  digitalWrite(dirPinY,HIGH);
  for(int y = 0; y < 900; y++) {
      digitalWrite(stepPinY,HIGH); 
      delayMicroseconds(Speed); 
      digitalWrite(stepPinY,LOW); 
      delayMicroseconds(Speed); 
  }
  digitalWrite(EnableY,HIGH);
  digitalWrite(EnableX,LOW);
  digitalWrite(dirPinX,LOW); //Changes the rotations direction
  //Makes 400 pulses for making two full cycle rotation
  for(int x = 0; x < 900; x++) {      
      digitalWrite(stepPinX,HIGH);
      delayMicroseconds(Speed);
      digitalWrite(stepPinX,LOW);
      delayMicroseconds(Speed);
  }
  digitalWrite(EnableX,HIGH);  
  TopX = 0;
  TopY = 0;
}  
    
void loop() 
{   
  if(start_flag == 0){
    start_flag = 1;
    digitalWrite(EnableY,LOW);
    digitalWrite(dirPinY,LOW); //Changes the rotations direction
    for(int y = 0; y < 12000; y++) {
      if(digitalRead(SwitchTopY)){
        digitalWrite(stepPinY,HIGH);
        delayMicroseconds(Speed);
        digitalWrite(stepPinY,LOW);
        delayMicroseconds(Speed);
      }
      else{
        break;
      }
    }
    digitalWrite(EnableY,HIGH); 
    digitalWrite(EnableX,LOW);
    digitalWrite(dirPinX,HIGH);
    for(int x = 0; x < 12000; x++) {
      if(TopX == 0){
        digitalWrite(stepPinX,HIGH); 
        delayMicroseconds(Speed); 
        digitalWrite(stepPinX,LOW); 
        delayMicroseconds(Speed); 
      }
      else{
        break;
      }
    }
    digitalWrite(EnableX,HIGH);
  }
  // Setting RED (R) filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  
  // Reading the output frequency
  redFrequency = pulseIn(sensorOut, LOW);
  Serial.print("R = ");
  Serial.print(redFrequency);
  Serial.print("TopX = ");
  Serial.print(TopX);
  Serial.print("TopY = ");
  Serial.println(TopY);
  if(redFrequency < 120){
    delay(3000);
    digitalWrite(S2,LOW);
    digitalWrite(S3,LOW);
    
    // Reading the output frequency
    redFrequency = pulseIn(sensorOut, LOW);
     // Printing the RED (R) value
    Serial.print("R = ");
    Serial.print(redFrequency);
    delay(100);
    
    // Setting GREEN (G) filtered photodiodes to be read
    digitalWrite(S2,HIGH);
    digitalWrite(S3,HIGH);
    // Reading the output frequency
    greenFrequency = pulseIn(sensorOut, LOW);
    
    // Printing the GREEN (G) value  
    Serial.print(" G = ");
    Serial.print(greenFrequency);
    delay(100);
   
    // Setting BLUE (B) filtered photodiodes to be read
    digitalWrite(S2,LOW);
    digitalWrite(S3,HIGH);
    
    // Reading the output frequency
    blueFrequency = pulseIn(sensorOut, LOW);
    
    // Printing the BLUE (B) value 
    Serial.print(" B = ");
    Serial.println(blueFrequency);
    delay(100);
    int lt[3] = {redFrequency, greenFrequency, blueFrequency}; 
    int minIndex = 0;
    int min = lt[minIndex];
    for (int i=1; i<3; i++){
      if (min>lt[i]){
        minIndex = i;
        min = lt[i];
      }
    }
   
    Serial.println();
    Serial.print(min);
    Serial.print( "=");
    Serial.println(minIndex);
    int cycleX = 0;
    if(minIndex == 0){
      cycleX = 2000;
    }
    else if(minIndex == 1){
      cycleX = 6000;
    }
    else if(minIndex == 2){
      cycleX = 10000;
    }
    for (posBottom = 0; posBottom <= 90; posBottom += 1) { // goes from 5 degrees to 90 degrees
      servoBottom.write(posBottom);              // tell servo to go to position in variable 'pos'
      delay(30);                       // waits 15ms for the servo to reach the position
    }
    digitalWrite(EnableY,LOW);
    digitalWrite(dirPinY,HIGH);
    for(int y = 0; y < 10000; y++) {
        digitalWrite(stepPinY,HIGH); 
        delayMicroseconds(Speed); 
        digitalWrite(stepPinY,LOW); 
        delayMicroseconds(Speed); 
    }
    digitalWrite(EnableY,HIGH);
    digitalWrite(EnableX,LOW);
    digitalWrite(dirPinX,LOW); //Changes the rotations direction
    //Makes 400 pulses for making two full cycle rotation
    for(int x = 0; x < cycleX; x++) {      
        digitalWrite(stepPinX,HIGH);
        delayMicroseconds(Speed);
        digitalWrite(stepPinX,LOW);
        delayMicroseconds(Speed);
    }
    digitalWrite(EnableX,HIGH);
    for (posTop = 0; posTop <= 45; posTop += 1) { // goes from 0 degrees to 180 degrees
      servoTop.write(posTop);              // tell servo to go to position in variable 'pos'
      delay(30);                       // waits 15ms for the servo to reach the position
    }
    delay(3000);
    
    for (posTop = 45; posTop >= 0; posTop -= 1) { // goes from 180 degrees to 0 degrees
      servoTop.write(posTop);              // tell servo to go to position in variable 'pos'
      delay(30);                       // waits 15ms for the servo to reach the position
    }    
    TopX = 0;
    TopY = 0;
    Serial.print("TopX = ");
    Serial.print(TopX);
    Serial.print("TopY = ");
    Serial.println(TopY);
    digitalWrite(EnableX,LOW);
    digitalWrite(dirPinX,HIGH);
    for(int x = 0; x < 12000; x++) {
      if(TopX == 0){
        digitalWrite(stepPinX,HIGH); 
        delayMicroseconds(Speed); 
        digitalWrite(stepPinX,LOW); 
        delayMicroseconds(Speed); 
        TopY = 0;
      }
      else{
        break;
      }
    }
    digitalWrite(EnableX,HIGH);
    digitalWrite(EnableY,LOW);
    digitalWrite(dirPinY,LOW); //Changes the rotations direction
    for(int y = 0; y < 12000; y++) {
      if(digitalRead(SwitchTopY)){
        digitalWrite(stepPinY,HIGH);
        delayMicroseconds(Speed);
        digitalWrite(stepPinY,LOW);
        delayMicroseconds(Speed);
        TopX = 0;
      }
      else{
        break;
      }
    }
    digitalWrite(EnableY,HIGH);
    for (posBottom = 90; posBottom >= 0; posBottom -= 1) { // goes from 180 degrees to 0 degrees
      servoBottom.write(posBottom);              // tell servo to go to position in variable 'pos'
      delay(30);                       // waits 15ms for the servo to reach the position
    } 
    
    delay(1000);
  }
  delay(1000);
  TopX = 0;
  TopY = 0;
}  
void limittopx(){
  TopX = 1;
  
    Serial.println("yesx");
}


void limittopy(){
  TopY = 1;
  
    Serial.println("yesy");
}
