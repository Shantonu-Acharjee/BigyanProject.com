#include <Servo.h>
 
Servo myservo;  // create servo object to control a servo
                // a maximum of eight servo objects can be created
 
int pos = 0;    // variable to store the servo position
 
void setup()
{
  myservo.attach(6);  // attaches the servo on pin 9 to the servo object
myservo.write(0); 
}
 
 
void loop()
{
  for(pos = 0; pos < 40; pos += 1)  // goes from 0 degrees to 180 degrees
  {                                  // in steps of 1 degree
    myservo.write(90);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  delay(3000);
  for(pos = 40; pos>=0; pos-=1)     // goes from 180 degrees to 0 degrees
  {
    myservo.write(0);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  delay(3000);
}
