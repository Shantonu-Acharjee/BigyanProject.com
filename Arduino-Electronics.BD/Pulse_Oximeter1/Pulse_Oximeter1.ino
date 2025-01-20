/*
   -------------------------------------------------------------------------------------
   HX711_ADC
   Arduino library for HX711 24-Bit Analog-to-Digital Converter for Weight Scales
   Olav Kallhovd sept2017
   -------------------------------------------------------------------------------------
*/

/*
   Settling time (number of samples) and data filtering can be adjusted in the config.h file
   For calibration and storing the calibration value in eeprom, see example file "Calibration.ino"

   The update() function checks for new data and starts the next conversion. In order to acheive maximum effective
   sample rate, update() should be called at least as often as the HX711 sample rate; >10Hz@10SPS, >80Hz@80SPS.
   If you have other time consuming code running (i.e. a graphical LCD), consider calling update() from an interrupt routine,
   see example file "Read_1x_load_cell_interrupt_driven.ino".

   This is an example sketch on how to use this library
*/

#include <HX711_ADC.h> //Including wait sensor library
#if defined(ESP8266)|| defined(ESP32) || defined(AVR)// Arduino Uno board detecting
#include <EEPROM.h> // Including EEPRom library
#endif
#include <Servo.h> // including Servo motor Library

Servo myservo; // define Servo motor
//pins:
const int HX711_dout = 6; //mcu > HX711 dout pin
const int HX711_sck = 7; //mcu > HX711 sck pin

//HX711 constructor:
HX711_ADC LoadCell(HX711_dout, HX711_sck);

const int calVal_calVal_eepromAdress = 0; // Variables
unsigned long t = 0; // variables
#include <Wire.h> //Including wire library
#include "MAX30100_PulseOximeter.h" // Including heart Sensor Library
PulseOximeter pox; // define heart Sensor
int pos = 0;
void setup() {
  Serial.begin(9600); delay(10);// set boud rate for communication. 
  Serial.println();
  Serial.println("Starting...");
  myservo.attach(3); // servo motor attached in pin number 3
  myservo.write(150); // rotate motor 150 degree
  float calibrationValue; // calibration value
  calibrationValue = 696.0; // uncomment this if you want to set this value in the sketch
#if defined(ESP8266) || defined(ESP32)
  //EEPROM.begin(512); // uncomment this if you use ESP8266 and want to fetch this value from eeprom
#endif
  //EEPROM.get(calVal_eepromAdress, calibrationValue); // uncomment this if you want to fetch this value from eeprom

  LoadCell.begin(); // load sensor begin.
  //LoadCell.setReverseOutput();
  unsigned long stabilizingtime = 2000; // tare preciscion can be improved by adding a few seconds of stabilizing time
  boolean _tare = true; //set this to false if you don't want tare to be performed in the next step
  LoadCell.start(stabilizingtime, _tare);
  if (LoadCell.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
  }
  else {
    LoadCell.setCalFactor(calibrationValue); // set calibration factor (float)
    Serial.println("Startup is complete");
  }
  while (!LoadCell.update());
  Serial.print("Calibration value: ");
  Serial.println(LoadCell.getCalFactor());
  Serial.print("HX711 measured conversion time ms: ");
  Serial.println(LoadCell.getConversionTime());
  Serial.print("HX711 measured sampling rate HZ: ");
  Serial.println(LoadCell.getSPS());
  Serial.print("HX711 measured settlingtime ms: ");
  Serial.println(LoadCell.getSettlingTime());
  Serial.println("Note that the settling time may increase significantly if you use delay() in your sketch!");
  if (LoadCell.getSPS() < 7) {
    Serial.println("!!Sampling rate is lower than specification, check MCU>HX711 wiring and pin designations");
  }
  else if (LoadCell.getSPS() > 100) {
    Serial.println("!!Sampling rate is higher than specification, check MCU>HX711 wiring and pin designations");
  }
  if (!pox.begin()) {
  Serial.println("FAILED");
  for(;;);
  } else {
  Serial.println("SUCCESS");
  }
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
  //pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop() {
  pox.update(); // update everytime when loop start
  static boolean newDataReady = 0;
  const int serialPrintInterval = 1000; //increase value to slow down serial print activity

  // check for new data/start next conversion:
  if (LoadCell.update()) newDataReady = true;

  // get smoothed value from the dataset:
  if (newDataReady) {
    if (millis() > t + serialPrintInterval) {
      
      float HeartRate = pox.getHeartRate();
      int SpO2 = pox.getSpO2();
      float i = LoadCell.getData();
      //Serial.print("Load_cell output val: ");
      //Serial.print(i);
      //Serial.println(HeartRate);
      newDataReady = 0;
      if(i>10) // 10g load limit
      {
        myservo.write(5);
      }
      else{
        myservo.write(150);
      }
      String str =String(HeartRate)+String("=")+String(i)+String("=")+String(SpO2);
      Serial.println(str);
      t = millis();
    }
  }

}
