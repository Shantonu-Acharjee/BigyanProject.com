
#include <math.h>// Library for math function 
//Header Declearation End

//Pin Assignment and declearation Start
 //data pin  DQ pin of DS18B20 connected to digital pin D5
  //set the LCD address to 0x27 for a 20 chars and 4 line display
const int analogPhPin = A0; //PH module pin P0 connected to analog pin A0

long phTot, temTot;
float phAvg, temAvg;
int x;
const float C = 21.34; //Constant of straight line (Y = mx + C)
const float m = -5.70; // Slope of straight line (Y = mx + C)
//Pin Assignment and declearation end

// start for generate custom character
byte customChar[] = {
  B00100,
  B00100,
  B11111,
  B00100,
  B00100,
  B00000,
  B11111,
  B00000
};
//End for generate custom character

//Setup Function Start 
void setup() {
  Serial.begin(9600); 
}
//Setup Function End

//Main function Start
void loop() {
  phTot = 0;
  temTot = 0;
  phAvg = 0;
  temAvg = 0;

  //taking 10 sample and adding with 10 milli second delay
  for(x=0; x<10 ; x++)
    {
        phTot += analogRead(A0);
        delay(10);
    }
    
    float phAvg = phTot/10;
     //convert sensor reading into milli volt
    float phVoltage =  phAvg * (5.0 / 1023.0); //convert sensor reading into milli volt

    float pHValue = phVoltage*m+C;
   Serial.println(pHValue); 
  delay(500);
}
