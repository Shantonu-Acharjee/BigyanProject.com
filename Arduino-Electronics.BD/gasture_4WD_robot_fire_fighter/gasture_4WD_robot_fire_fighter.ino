/*
 * Created by Electronics.Com.BD
 * Project: 4WD test
 */
#include <SPI.h>
#include <NRFLite.h>
#include <Servo.h>

#define IN_11  2    // L298N #1 in 1 motor Right Ahead
#define IN_12  3    // L298N #1 in 2 motor Right Ahead
#define IN_13  4    // L298N #1 in 3 motor Right Back
#define IN_14  7    // L298N #1 in 4 motor Right Back
//Initializing the variables
boolean x = 0;
int directionOfMovement = 0;
const int motorAspeed  = 5;
const int motorBspeed  =6;
const int vSpeed = 150;
const int trigPin = 19;
const int echoPin = 18;
const int firePin = 17;
const int servoPin = 16;
const int relayPin =15;
long duration;
int distance;
int pos = 0;
int flame = 1;
String message;
const static uint8_t RADIO_ID = 0;       // Our radio's id.  The transmitter will send to this id.
const static uint8_t PIN_RADIO_CE = 8;
const static uint8_t PIN_RADIO_CSN = 9;
Servo myservo;  // create Servo object to control a servo
struct RadioPacket { // Any packet up to 32 bytes can be sent.

  uint8_t FromRadioId;
  uint32_t Data;
  uint32_t FailedTxCount;

};
//Create NRF object
NRFLite _radio;
RadioPacket _radioData;

void setup() {  
   Serial.begin(9600);  
   pinMode(IN_11, OUTPUT);
   pinMode(IN_12, OUTPUT);
   pinMode(IN_13, OUTPUT);
   pinMode(IN_14, OUTPUT);  
   pinMode(relayPin, OUTPUT);
   pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
   pinMode(echoPin, INPUT); // Sets the echoPin as an Input 
   pinMode(firePin, INPUT);
   analogWrite (motorAspeed, vSpeed);
   analogWrite (motorBspeed, vSpeed);
   digitalWrite(relayPin, HIGH);
   myservo.attach(servoPin);  // attaches the servo on to the Servo object
   myservo.write(90);
   if (!_radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN)) {
    Serial.println("Cannot communicate with radio");
    while (1); // Wait here forever.
   }
}

void loop(){
  while (_radio.hasData()) {
    _radio.readData(&_radioData); // Note how '&' must be placed in front of the variable name.
    message = _radioData.Data;
    Serial.println(message);
    directionOfMovement = message.toInt();
    moveAccordingly();
    delay(200);
  }
}
//this function moves the car according to the message
void moveAccordingly() {
  checkFire();
  if (directionOfMovement == 1) {
    // Clears the trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);    
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);    
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);    
    // Calculating the distance
    distance= duration*0.034/2;    
    // Prints the distance on the Serial Monitor
    Serial.print("Distance: ");
    Serial.println(distance);
    if(distance >= 20){
      goAhead();
      Serial.println("front");
    }
    else{
      stopRobot();
    }
  }
  else if (directionOfMovement == 2) {
    goBack();
    Serial.println("back");
  }
  else if (directionOfMovement == 3) {
    goLeft();
    Serial.println("left");
  }
  else if (directionOfMovement == 4) {
    goRight();
    Serial.println("right");
  }
  else if (directionOfMovement == 0) {
    stopRobot();
    Serial.println("none");
  }
}

void checkFire(){
  int flame = digitalRead(firePin);
  if(flame == 0){
    directionOfMovement = 0;
    stopRobot();
    spray();
  }
}
void spray(){
  digitalWrite(relayPin, LOW);
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
  digitalWrite(relayPin, HIGH);
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
