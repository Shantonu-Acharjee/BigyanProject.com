/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
int btn = 2;
unsigned long previousMillis = 0;        // will store last time LED was updated

// constants won't change :
const unsigned long interval = 8UL*60UL*60UL*1000UL;
void setup() {
  Serial.begin(9600);
  pinMode(btn,INPUT_PULLUP);
  myservo.attach(11);  // attaches the servo on pin 9 to the servo object
  myservo.write(90);
  delay(15);
}

void loop() {
  unsigned long currentMillis = millis();
  if(digitalRead(btn) == LOW){
    Serial.println(digitalRead(btn)); 
    for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
    for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
    previousMillis = currentMillis;
  }

  if (currentMillis - previousMillis >= interval) {
       Serial.println("time"); 
    // save the time you should have toggled the LED
    previousMillis += interval;

    for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
    for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
  } 
}
