#include <AFMotor.h>

AF_DCMotor motor(4);

void setup(){
	
	motor.setSpeed(200); //Set initial speed of the motor & stop
	motor.run(RELEASE); //for stop the motor
}

void loop(){
	
	motor.run(FORWARD);
	delay(1000);
	
  motor.run(RELEASE);
	delay(1000);
	
	motor.run(BACKWARD);
	delay(1000);
}
