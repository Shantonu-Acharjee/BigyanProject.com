/*
*
* Read and write demo of the AT24CX library
* Written by Christian Paul, 2014-11-24
* 
* 
*/

// include libraries
#include <Wire.h>
#include <AT24CX.h>

// EEPROM object
AT24CX mem;

// setup
void setup() {
  // serial init
  Serial.begin(9600);
  Serial.println("AT24CX read/write demo");
  Serial.println("----------------------");
    // read and write integer
  Serial.println("Write 65000 to address 15");
  mem.writeInt(15, 9999);
  Serial.println("Read integer from address 15 ...");
  unsigned int i = mem.readInt(15);
  Serial.print("... read: ");
  Serial.println(i, DEC);
  Serial.println();
}

// main loop
void loop() {

  



}
