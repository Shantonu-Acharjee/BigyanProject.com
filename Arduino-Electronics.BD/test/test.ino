/*
  Button

 Turns on and off a light emitting diode(LED) connected to digital
 pin 13, when pressing a pushbutton attached to pin 2.


 The circuit:
 * LED attached from pin 13 to ground
 * pushbutton attached to pin 2 from +5V
 * 10K resistor attached to pin 2 from ground

 * Note: on most Arduinos there is already an LED on the board
 attached to pin 13.


 created 2005
 by DojoDave <http://www.0j0.org>
 modified 30 Aug 2011
 by Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/Button
 */

// constants won't change. They're used here to
// set pin numbers:
const int buttonPin1 = A0;
const int buttonPin2 = A1;
const int buttonPin3 = A2;
const int buttonPin4 = A3;
const int buttonPin5 = A4;
const int buttonPin6 = A5;     // the number of the pushbutton pin
const int ledPin1 =  7;      // the number of the pushbutton pin
const int ledPin2 =  0;     // the number of the pushbutton pin
const int ledPin3 =  1;     // the number of the pushbutton pin
const int ledPin4 =  2;     // the number of the pushbutton pin
const int ledPin5 =  3;      // the number of the pushbutton pin
const int ledPin6 =  4;      // the number of the pushbutton pin
const int ledPin7 =  5;     // the number of the pushbutton pin
const int ledPin8 =  6;     // the number of the pushbutton pin
const int ledPin9 =  7;     // the number of the pushbutton pin
const int ledPin10 =  9;     // the number of the pushbutton pin
const int ledPin11 =  10;    // the number of the LED pin

// variables will change:
int buttonState1 = 0;         // variable for reading the pushbutton status
int buttonState2 = 0;         // variable for reading the pushbutton status
int buttonState3 = 0;         // variable for reading the pushbutton status
int buttonState4 = 0;         // variable for reading the pushbutton status
int buttonState5 = 0;         // variable for reading the pushbutton status
int buttonState6 = 0;         // variable for reading the pushbutton status

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);
  pinMode(ledPin5, OUTPUT);
  pinMode(ledPin6, OUTPUT);
  pinMode(ledPin7, OUTPUT);
  pinMode(ledPin8, OUTPUT);
  pinMode(ledPin9, OUTPUT);
  pinMode(ledPin10, OUTPUT);
  pinMode(ledPin11, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);
  pinMode(buttonPin4, INPUT);
  pinMode(buttonPin5, INPUT);
  pinMode(buttonPin6, INPUT);
}

void loop() {
  // read the state of the pushbutton value:
  buttonState1 = digitalRead(buttonPin1);
  buttonState2 = digitalRead(buttonPin2);
  buttonState3 = digitalRead(buttonPin3);
  buttonState4 = digitalRead(buttonPin4);
  buttonState5 = digitalRead(buttonPin5);
  buttonState6 = digitalRead(buttonPin6);

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState1 == HIGH) {
    digitalWrite(ledPin1, HIGH);
    digitalWrite(ledPin2, HIGH);
  } else if (buttonState2 == HIGH) {
    digitalWrite(ledPin3, HIGH);
    digitalWrite(ledPin4, HIGH);
  } else if (buttonState3 == HIGH) {
    digitalWrite(ledPin5, HIGH);
    digitalWrite(ledPin6, HIGH);
  } else if (buttonState4 == HIGH) {
    digitalWrite(ledPin7, HIGH);
    digitalWrite(ledPin8, HIGH);
  } else if (buttonState5 == HIGH) {
    digitalWrite(ledPin9, HIGH);
    digitalWrite(ledPin10, HIGH);
  } else if (buttonState6 == HIGH) {
    digitalWrite(ledPin11, HIGH);
  } else {
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, LOW);
    digitalWrite(ledPin3, LOW);
    digitalWrite(ledPin4, LOW);
    digitalWrite(ledPin5, LOW);
    digitalWrite(ledPin6, LOW);
    digitalWrite(ledPin7, LOW);
    digitalWrite(ledPin8, LOW);
    digitalWrite(ledPin9, LOW);
    digitalWrite(ledPin10, LOW);
    digitalWrite(ledPin11, LOW);
  }
}
