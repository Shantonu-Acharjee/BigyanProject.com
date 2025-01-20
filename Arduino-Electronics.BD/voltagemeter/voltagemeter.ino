/*
  LiquidCrystal Library - display() and noDisplay()

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD and uses the
 display() and noDisplay() functions to turn on and off
 the display.

 The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystalDisplay

 */

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
// The main loop will sleep for X milliseconds
int loopDelayMillis = 100;
// This is the pin in for amps
int sensorPin = 1;
// The current amps
int amps = 0;
// Total amp hours consumed
float ampHours = 0.0;
// The state of charge percentage
int socPercent = 100;
// maximum amp hours of your battery pack
int maxAmpHours = 100;
// The current loop timestamp
unsigned long currentTime = millis();
// The last calulation timestamp
unsigned long lastCalculationTime = currentTime;
// The last display update second
unsigned long lastSec = 0;
// the total amps used in our sample period
int ampSampleTotal = 0;
// the number of samples made
int sampleCount = 0;
//This is the waveform history, 15 seconds worth
int waveHist[15];
//These will be the battery characters as bitmaps.
byte battery1[8];
byte battery2[8];
byte battery3[8];
byte battery4[8];
byte battery5[8];

// the empty power waveform character bitmaps         
byte wave1[8];
byte wave2[8];
byte wave3[8];

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(20, 4);
  lcd.clear();      // this clears the LCD. You can use this at any time
  
  lcd.print("Prototype");
  lcd.setCursor(0,1);
  lcd.print("Fuel Gauge");
}             

/**
 * This is the main loop.
 */
void loop()
{
  currentTime = millis();
  
  //add the amps now so we can average them later
  amps = analogRead(sensorPin);
  ampSampleTotal = ampSampleTotal + amps;
  sampleCount++;
  
  //update the display when when the seconds change,
  //the lcd can not handle much faster
  int secs = (currentTime / 1000) % 60;
  if (secs != lastSec) {
      updateFuelConsumption();
      updateDisplay();
      lastSec = secs;
  }
  
  //and sleep for a while
  delay(loopDelayMillis);
}

/**
 * Calculates the fuel used since the previous time. We have to assume the amps have been constant. 
 * Decrease the loopDelayMillis to increase accuracy.
 */
void updateFuelConsumption() {
  // use the average amps then add the amps hours we have used in the last X milliseconds (60mins * delta secs)
  float averageAmps = (ampSampleTotal * 1.0) / sampleCount;
  // the number of milliseconds since the last calc
  int deltaMillis = currentTime - lastCalculationTime;
  // the time factor is needed to calculate the amp hours
  float timeFactor = 3600.0 * deltaMillis / 1000;
  ampHours = ampHours + (averageAmps / timeFactor);
  //ampHours = averageAmps / timeFactor;
  //update the state of charge percentage
  socPercent = ((maxAmpHours - ampHours) / (maxAmpHours * 1.0)) * 100;
  //we have finished the calculations, update the timestamp for the next calculation
  lastCalculationTime = currentTime;
  ampSampleTotal = 0;
  sampleCount = 0;
}

/**
 * Updates the LCD screen
 */
void updateDisplay () {
  createBatteryChars();
  createWaveformChars();

  lcd.clear();
  lcd.home();
  lcd.setCursor(0,0);
  
  printWaveform();
  printAmps();
  printBattery();
  printSOC();
  
  //move to second row
  lcd.setCursor(0,1);
  
  printAmpHours();
  printTime();
}

/**
 * Prints the waveform characters stored in special chars 1,2 and 3
 */
void printWaveform() {
  lcd.print((char)0);
  lcd.write(1);
  lcd.write(2);
}

/**
 * Prints the battery characters stored in special chars 3,4,5,6,7
 */
void printBattery() {
  lcd.write(3);
  lcd.write(4);
  lcd.write(5);
  lcd.write(6);
  lcd.write(7);
}

/**
 * Prints the current amp reading
 */
void printAmps() {
  if (amps > 999) {
    lcd.print("999");
  }
  else if (amps < 10) {
    lcd.print("  ");
    lcd.print(amps);
  }
  else if (amps < 100) {
    lcd.print(" ");
    lcd.print(amps);
  }
  else {
    lcd.print(amps);
  }
  lcd.print("A");
}

/**
 * Prints the time in mins and seconds
 */
void printTime() {
  int mins = currentTime / 60000;
  if (mins < 10) {
    lcd.print(" ");
  }
  lcd.print(mins);
  lcd.print(":");
  
  int secs = (currentTime / 1000) % 60;
  if (secs < 10) {
    lcd.print("0");
  } 
  lcd.print(secs);
}

/**
 * print battery state of charge (soc)%
 */
void printSOC() {
  if (socPercent == 100) {
    lcd.print("99%");
  }
  else {
    lcd.print(socPercent);
    lcd.print("%");
  }
}

/**
 * Print Amp Hours consumed.
 */
void printAmpHours() {
  if (ampHours < 10) {
    lcd.print(" ");
  }
  lcd.print(ampHours,2);
  lcd.print("Ah   ");
}

/**
 * The waveform chars are bitmaps. Each column is one second.
 * So we add 1 column and shift the others across.
 */
void createWaveformChars() {
  //clear all the waveforms chars to prevent artifacts
  for (int i=0; i < 8; i++) {
    wave1[i] = B00000;
    wave2[i] = B00000;
    wave3[i] = B00000;
  }
               
  //now shift the waveform history across one second
  for (int i=14; i >= 1; i--) {
    waveHist[i] = waveHist[i-1];
  }
  
  //scale the current amps to height of the waveform in pixels 
  //waveHist[0] = amps / 128;  //1024 / 8 =128
  waveHist[0] = amps / 114;  //1024 / 9 = 114
  
  //now do the first character
  for (int i=0; i < 5; i++) {
    // loop through the waveform history
    for (int k=0; k < 8; k++) {
      if (waveHist[i] >= 8-k) {
        bitSet(wave1[k],i);
      }
    }
  }
  
  //now the second character
  for (int i=5; i < 10; i++) {
    // loop through the waveform history
    for (int k=0; k < 8; k++) {
      if (waveHist[i] >= 8-k) {
        bitSet(wave2[k],i-5);
      }
    }
  }
  
  //now the third character
  for (int i=10; i < 15; i++) {
    // loop through the waveform history
    for (int k=0; k < 8; k++) {
      if (waveHist[i] >= 8-k) {
        bitSet(wave3[k],i-10);
      }
    }
  }
  
  lcd.createChar(2,wave1);
  lcd.createChar(1,wave2);
  lcd.createChar(0,wave3);
}

/**
 * Create the battery icon
 */
void createBatteryChars() {
  
  battery1[0] = B11111;
  battery1[1] = B10000; 
  battery1[2] = B10000;
  battery1[3] = B10000;
  battery1[4] = B10000;
  battery1[5] = B10000;
  battery1[6] = B10000;
  battery1[7] = B11111;
  
  battery2[0] = B11111;
  battery2[1] = B00000; 
  battery2[2] = B00000;
  battery2[3] = B00000;
  battery2[4] = B00000;
  battery2[5] = B00000;
  battery2[6] = B00000;
  battery2[7] = B11111;
  
  battery3[0] = B11111;
  battery3[1] = B00000; 
  battery3[2] = B00000;
  battery3[3] = B00000;
  battery3[4] = B00000;
  battery3[5] = B00000;
  battery3[6] = B00000;
  battery3[7] = B11111;
  
  battery4[0] = B11111;
  battery4[1] = B00000; 
  battery4[2] = B00000;
  battery4[3] = B00000;
  battery4[4] = B00000;
  battery4[5] = B00000;
  battery4[6] = B00000;
  battery4[7] = B11111;
  
  battery5[0] = B11000;
  battery5[1] = B01000; 
  battery5[2] = B01100;
  battery5[3] = B00100;
  battery5[4] = B00100;
  battery5[5] = B01100;
  battery5[6] = B01000;
  battery5[7] = B11000;
  
  for (int i=0; i < 4; i++) {
    if (socPercent >= ((i+1) * 5)) {
      bitSet(battery1[1],3-i);
      bitSet(battery1[2],3-i);
      bitSet(battery1[3],3-i);
      bitSet(battery1[4],3-i);
      bitSet(battery1[5],3-i);
      bitSet(battery1[6],3-i);
    }
  }
  
  for (int i=0; i < 5; i++) {
    if (socPercent >= ((i+4) * 5)) {
      bitSet(battery2[1],4-i);
      bitSet(battery2[2],4-i);
      bitSet(battery2[3],4-i);
      bitSet(battery2[4],4-i);
      bitSet(battery2[5],4-i);
      bitSet(battery2[6],4-i);
    }
  }
  
  for (int i=0; i < 5; i++) {
    if (socPercent >= ((i+9) * 5)) {
      bitSet(battery3[1],4-i);
      bitSet(battery3[2],4-i);
      bitSet(battery3[3],4-i);
      bitSet(battery3[4],4-i);
      bitSet(battery3[5],4-i);
      bitSet(battery3[6],4-i);
    }
  }
  
  for (int i=0; i < 5; i++) {
    if (socPercent >= ((i+14) * 5)) {
      bitSet(battery4[1],4-i);
      bitSet(battery4[2],4-i);
      bitSet(battery4[3],4-i);
      bitSet(battery4[4],4-i);
      bitSet(battery4[5],4-i);
      bitSet(battery4[6],4-i);
    }
  }
  
  if (socPercent >= 95) {
    bitSet(battery5[1],4);
    bitSet(battery5[2],4);
    bitSet(battery5[3],4);
    bitSet(battery5[4],4);
    bitSet(battery5[5],4);
    bitSet(battery5[6],4);
    bitSet(battery5[3],3);
    bitSet(battery5[4],3);
  }
  
  lcd.createChar(3,battery1);
  lcd.createChar(4,battery2);
  lcd.createChar(5,battery3);
  lcd.createChar(6,battery4);
  lcd.createChar(7,battery5);
}

