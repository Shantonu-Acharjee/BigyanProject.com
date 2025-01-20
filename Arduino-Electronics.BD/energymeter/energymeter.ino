/*
Measuring Current Using ACS712
*/
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int calVal = 512;
int mVpA = 61;

void setup() {
  lcd.begin(20, 4);
 lcd.clear(); 
}

void loop() {
 lcd.clear(); 
float average = 0;
 
for(int i = 0; i < 1000; i++) {
        average = average + (.0264 * analogRead(A0) -13.51);
        delay(1);
                                 }
lcd.print(average);
lcd.println("mA");
}
