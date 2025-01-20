#include <SoftwareSerial.h>
#include <Servo.h>
Servo myservo;
#define ESP 13
#define FlameSensor 10
#define ServoMotor 11
int countTrueCommand;
int countTimeCommand; 
boolean found = false; 
int valSensor = 1;
const int trigPin = 6;
const int echoPin = 5;
long duration;
int distance;
int pos = 0;    // variable to store the servo position
boolean OPStatus = false;
void setup(){
  Serial.begin(9600); 
  pinMode(4, OUTPUT); 
  digitalWrite(4, LOW);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(7, OUTPUT); 
  digitalWrite(7, HIGH);
  pinMode(8, OUTPUT); 
  digitalWrite(8, HIGH);
  pinMode(9, OUTPUT); 
  digitalWrite(9, LOW);
  pinMode(ESP, OUTPUT); 
  digitalWrite(ESP, LOW);
  pinMode(FlameSensor, INPUT); 
  myservo.attach(ServoMotor);
  myservo.write(0);   
}
void loop()
{
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
  Serial.print(distance);
  if(distance < 20 && OPStatus == false){ 
    Serial.print(" Dish Detected "); 
    Serial.print(" Turning Gas On... "); 
    gasOn();
    delay(4000);
    if(digitalRead(FlameSensor) == LOW){
      OPStatus = true;
    }
    else{  
      Serial.print(" Flame Not Detected "); 
      Serial.print(" Turning Gas Off... "); 
      gasOff();
      OPStatus = false;
    }
    
  }
  else if(distance < 20 && OPStatus == true){
      Serial.print(" Cooking.. ");      
  }
  else if(distance > 20 && OPStatus == true){  
      Serial.print(" Dish Not Detected "); 
      Serial.print(" Turning Gas Off... "); 
      gasOff();
      OPStatus = false;
  }
  else{   
    Serial.print(" Dish Not Detected "); 
    OPStatus = false;
  }
  if(OPStatus == true){  
    Serial.println(" Sending data to the server... ");  
    digitalWrite(ESP, HIGH);
    delay(1000);
    Serial.println(" Data send. ");
  }
  else{
    digitalWrite(ESP, LOW);    
  }
  Serial.println(); 
  delay(5000);
}
void gasOn(){
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  Serial.print(" Gas ON ");
}
void gasOff(){
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  Serial.print(" Gas OFF ");
}
