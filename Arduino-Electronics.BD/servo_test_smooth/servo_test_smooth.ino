#include <Servo.h>

Servo motor_1;
int pos = 0;
void setup() {
  motor_1.attach(9, 544, 2500);

}

void loop() {
  for(pos = 544; pos < 2500; pos += 1)  // goes from 0 degrees to 180 degrees
  { 
    motor_1.writeMicroseconds(pos);
    delay(5);
  }
  for(pos = 2500; pos>=544; pos-=1)     // goes from 180 degrees to 0 degrees
  {
    motor_1.write(pos);              // tell servo to go to position in variable 'pos'
    delay(5);                       // waits 15ms for the servo to reach the position
  }
}
