#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
// defines pins numbers
const int trigPin = 5;
const int echoPin = 6;
const int LEDPinRED = 10;
const int LEDPinGREEN = 9;
RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";
// defines variables
long duration;
int distance;
unsigned long delayStart = 0; // the time the delay started

void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(LEDPinRED, OUTPUT);
  pinMode(LEDPinGREEN, OUTPUT);
  digitalWrite(LEDPinRED,LOW);
  digitalWrite(LEDPinGREEN,LOW);
  Serial.begin(9600); // Starts the serial communication
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculating the distance
  distance= duration*0.034/2;
  
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
  if ((millis() - delayStart) >= 1000) {
    delayStart += 1000;
    radio.write(&distance, sizeof(int));
    if(distance < 6)
    {
      digitalWrite(LEDPinRED,HIGH);
      digitalWrite(LEDPinGREEN,LOW);
    }
    else
    {
      digitalWrite(LEDPinRED,LOW);
      digitalWrite(LEDPinGREEN,HIGH);
    }
  }
}
