/*
  myServo.cpp
  Code by lingib
  https://www.instructables.com/member/lingib/instructables/
  Last update 23 December 2020

  ----------
  COPYRIGHT
  ----------
  This code is free software: you can redistribute it and/or
  modify it under the terms of the GNU General Public License as published
  by the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This software is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License. If
  not, see <http://www.gnu.org/licenses/>.
*/
 
#include "myServo.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

/////// Constructor ///////
myServo::myServo() {}

/////// attach() ////////
void myServo::attach(byte pin) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  _pin = pin;
}

//////// calibrate() ///////
void myServo::calibrate(unsigned long pulse_width){
  _width = pulse_width;
}

////////min() ///////
void myServo::min_width(unsigned long pulse_width){
  _min = pulse_width;
}

////////max() ///////
void myServo::max_width(unsigned long pulse_width){
  _max = pulse_width;
}

//////// angle() ///////
void myServo::angle(float angle, unsigned long deadband){
  _width = (unsigned long)((float)(_max - _min) * angle / 180) + _min + deadband;
}

//////// return servo pulse width ///////
unsigned long myServo::width(){
  return _width;
}

/////// set control line high ////////
void myServo::ctrl_high(){
   digitalWrite(_pin, HIGH);  
}

/////// set control line low ////////
void myServo::ctrl_low(){
   digitalWrite(_pin, LOW);  
}
