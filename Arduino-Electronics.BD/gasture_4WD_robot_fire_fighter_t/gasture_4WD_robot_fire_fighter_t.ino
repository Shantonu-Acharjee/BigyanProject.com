#include <Wire.h>
#include <TinyMPU6050.h>
#include <SPI.h>
#include <NRFLite.h>
#include "MAX30105.h"
#include "heartRate.h"

MPU6050 mpu (Wire);
MAX30105 particleSensor;

const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred

float beatsPerMinute;
int beatAvg;

int message;

const static uint8_t RADIO_ID = 1;             // Our radio's id.
const static uint8_t DESTINATION_RADIO_ID = 0; // Id of the radio we will transmit to.
const static uint8_t PIN_RADIO_CE = 7;
const static uint8_t PIN_RADIO_CSN = 8;

struct RadioPacket { // Any packet up to 32 bytes can be sent.

  uint8_t finger;
  uint8_t FromRadioId;
  uint32_t Data;
  float BeatsPerMinute;
  uint32_t FailedTxCount;

};

//Create NRF24 object
NRFLite _radio;
RadioPacket _radioData;

void setup() {
  pinMode(2,INPUT);
  pinMode(3,OUTPUT);
  digitalWrite(3,HIGH);
  // Initialization
  mpu.Initialize();
  // Calibration (wait for about 20s to calibrate)
  mpu.Calibrate();

  //start up
  Serial.begin(115200);
  // Initialize sensor
  if(!particleSensor.begin(Wire, I2C_SPEED_FAST)){
    Serial.println("MAX30102 was not found. Please check wiring/power. ");
    while (1);
  }
  Serial.println("Place your index finger on the sensor with steady pressure.");

  particleSensor.setup(); //Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0); //Turn off Green LED
  Serial.println("Done Calibration");

  if (!_radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN)) {
    Serial.println("Cannot communicate with radio");
    while (1); // Wait here forever.
  }

  _radioData.FromRadioId = RADIO_ID;

}

void loop() {

  mpu.Execute();

  while (mpu.GetRawAccX() <= -8000) {

    //send msg to move front
    message = 1;
    _radioData.Data = message;
    sendData();
    Serial.println("front");
    mpu.Execute();

  }

  while (mpu.GetRawAccX() >= 8000) {

    //send msg to move back
    message = 2;
    _radioData.Data = message;
    sendData();
    Serial.println("back");
    mpu.Execute();

  }

  while (mpu.GetRawAccY() <= -8000) {

    //send msg to move left
    message = 3;
    _radioData.Data = message;
    sendData();
    Serial.println("left");
    mpu.Execute();

  }

  while (mpu.GetRawAccY() >= 8000) {

    //send msg to move right
    message = 4;
    _radioData.Data = message;
    sendData();
    Serial.println("right");
    mpu.Execute();

  }

  while (mpu.GetRawAccX() < 8000 and mpu.GetRawAccX() > -8000 and mpu.GetRawAccY() < 8000 and mpu.GetRawAccY() > -8000) {

    //send msg to stop
    message = 0;
    _radioData.Data = message;
    sendData();
    //Serial.println("none");
    mpu.Execute();

  }

}

void sendData() {
  if(digitalRead(2)){    
    digitalWrite(3,LOW);
  }
  else{    
    digitalWrite(3,HIGH);
  }
  long irValue = particleSensor.getIR();

  if (checkForBeat(irValue) == true) {
    //We sensed a beat!
    long delta = millis() - lastBeat;
    lastBeat = millis();

    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute < 255 && beatsPerMinute > 20) {
      rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
      rateSpot %= RATE_SIZE; //Wrap variable

      //Take average of readings
      beatAvg = 0;
      for (byte x = 0 ; x < RATE_SIZE ; x++)
        beatAvg += rates[x];
      beatAvg /= RATE_SIZE;
    }
  }
  if (irValue < 50000){
    _radioData.finger = 0;
    _radioData.BeatsPerMinute = 0;
  }
  else{
    _radioData.finger = 1;
    _radioData.BeatsPerMinute = beatsPerMinute;
  }
  Serial.print(_radioData.Data);
  Serial.print("=");
  Serial.print(beatsPerMinute);
  if (_radio.send(DESTINATION_RADIO_ID, &_radioData, sizeof(_radioData))) { // Note how '&' must be placed in front of the variable name.

  }
  else {

    Serial.println("Failed");
    _radioData.FailedTxCount++;

  }

  //delay(500);
  mpu.Execute();

}
