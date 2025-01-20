/*
  myServo.h
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

#ifndef myServo_h
#define myServo_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class myServo {

  private:
    byte _pin;                          // Arduino pin
    unsigned long _width;               // pulsewidth representing target angle
    unsigned long _min;                 // pulsewidth representing 0 degrees
    unsigned long _max;                 // pulsewidth representing 180 degrees
     
  public:
    myServo();                                          // constructor
    void attach(byte pin);                              // attach servo
    void calibrate(unsigned long pulse_width);          // servo calibration
    void min_width(unsigned long pulse_width);          // servo calibration
    void max_width(unsigned long pulse_width);          // servo calibration
    void angle(float angle, unsigned long correction);  // angle in degrees, deadband correction in uS
    void refresh();                                     // refresh servo

    // ----- parallel refresh
    unsigned long width();              // return servo pulse width
    void ctrl_high();                   // leading control edge
    void ctrl_low();                    // trailing control edge    
};

#endif
