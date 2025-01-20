/*
 AC Light Control
 Ryan McLaughlin <ryanjmclaughlin@gmail.com>
 with slight modifications
*/

#include <TimerOne.h>           // http://www.arduino.cc/playground/Code/Timer1
#define PINS 3
volatile int pinCount[PINS];    // make volatile to make available in interrupt
volatile boolean zero_cross=0;  // Boolean to store a "switch" to tell us if we have crossed zero
int AC_pins[] = {3,4,5};        // Stup the pin numbers
int AC_dim[PINS];               // Holds Dimming levels (0-128)  0 = on, 128 = 0ff
int freqStep = 78;              // Set the delay for the frequency of power (65 for 60Hz, 78 for 50Hz) per step (using 128 steps)
                                // freqStep may need some adjustment depending on your power the formula 
                                // you need to us is (500000/AC_freq)/NumSteps = freqStep
                                
void setup() {
  for(int a=0; a < PINS; a++) { //set the pins to output
   pinMode(AC_pins[a],OUTPUT);
   pinCount[a] = 0;             // keeps track of the time in the cycle 
   AC_dim[a] = 0;               // dimming level set to zero
  }
  attachInterrupt(0, zero_cross_detect, FALLING);  // Attach Interrupt to Pin 2 (interrupt 0) for Zero Cross Detection
  Serial.begin(9600);
  Timer1.initialize(freqStep);                     // Initialize TimerOne library for the freq we need
  Timer1.attachInterrupt(dim_check, freqStep);     // Use the TimerOne Library to attach an interrupt
                                                   // to the function we use to check to see if it is 
                                                   // the right time to fire the triac.  This function 
                                                   // will now run every freqStep in microseconds.                                            
}

void zero_cross_detect() {        // function to be fired at the zero crossing                           
   zero_cross = 1;                // set flag to tell dimming function zero cross has occured
}                                 // End zero_cross_detect

void dim_check() {                   // Function will fire the triac at the proper time
 if(zero_cross == 1) {               // First check to make sure the zero-cross has happened else do nothing
   for(int a=0; a < PINS; a++) {
     if(pinCount[a] >= AC_dim[a]) {       // Check and see if i has reached the dimming value we want
       digitalWrite(AC_pins[a], HIGH);    // Fire the Triac 
       delayMicroseconds(5);              // Pause briefly to ensure the triac turned on
       digitalWrite(AC_pins[a], LOW);     // Turn off the Triac gate (Triac will turn off at the next zero cross)
       pinCount[a] = 0;                   // Reset the accumulator
       zero_cross = 0;                    // Reset the zero_cross so it may be turned on again at the next zero_cross_detect    
     } else {
       pinCount[a]++;                     // If the dimming value has not been reached, incriment the counter
     }    
   }
 }
}

void loop() {
  // This is simply making all outputs cycle through bright-dark, out of time with each other.
 for(int i=0; i<127; i ++) {
   for(int a=0; a < PINS; a++) {
      int ii = i+42;               //this is the bit that puts the blinking lights out of sync with one another
      if(ii > 127) ii -= 127;
      AC_dim[a] = ii;
    }
    delay(50);
  }
}
