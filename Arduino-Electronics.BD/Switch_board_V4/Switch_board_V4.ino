#include <IRremote.h>
#define latchPin A1
#define clockPin A0
#define dataPin A2
#define lastLED 13
int IRPIN = 10;
// Used for generating interrupts using CLK signal
const int PinA = 3; 
// Used for reading DT signal
const int PinB = 11;
// Used for the push button switch
const int PinSW = 12; 
// Keep track of last rotary value
int lastCount = 50;
// Updated by the ISR (Interrupt Service Routine)
volatile int dimPosition = 111;
int LOAD_status = HIGH;
int reading_PinSW; 
int previous_PinSW = LOW;
long time = 0;         // the last time the output pin was toggled
long debounce = 200;   // the debounce time, increase if the output flickers
int SignalOUT = 9;
int SwitchOUT = 7;
int Light = 4;
int Light_status = LOW;

byte i;
byte j;
int LEDposition[15] = {30};
IRrecv irrecv(IRPIN);

decode_results results;

// ------------------------------------------------------------------
// INTERRUPT     INTERRUPT     INTERRUPT     INTERRUPT     INTERRUPT 
// ------------------------------------------------------------------
void isr ()  {
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();

  // If interrupts come faster than 5ms, assume it's a bounce and ignore
  if (interruptTime - lastInterruptTime > 5) {
    if (digitalRead(PinB) == LOW)
    {
      dimPosition+=9 ; // Could be -5 or -10
    }
    else {
      dimPosition-=9 ; // Could be +5 or +10
    }
    // Restrict value from 0 to +255
    dimPosition = min(255, max(111, dimPosition));

    // Keep track of when we were here last (no more than every 5ms)
    lastInterruptTime = interruptTime;
  }
}

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
  // Rotary pulses are INPUTs
  pinMode(PinA, INPUT);
  pinMode(PinB, INPUT);
  // Switch is floating so use the in-built PULLUP so we don't need a resistor
  pinMode(PinSW, INPUT_PULLUP);
  pinMode(SwitchOUT, OUTPUT);
  pinMode(SignalOUT, OUTPUT);
  pinMode(Light, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
    // Attach the routine to service the interrupts
  attachInterrupt(digitalPinToInterrupt(PinA), isr, LOW);
  Serial.println("Start");
}

void loop(){
  if(dimPosition == 111)
  {
    digitalWrite(lastLED, LOW);
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, 30);
    digitalWrite(latchPin, HIGH);
  }
  else if(dimPosition == 120)
  {
    digitalWrite(lastLED, LOW);
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, 29);
    digitalWrite(latchPin, HIGH);
  }
  else if(dimPosition == 129)
  {
    digitalWrite(lastLED, LOW);
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, 27);
    digitalWrite(latchPin, HIGH);
  }
  else if(dimPosition == 138)
  {
    digitalWrite(lastLED, LOW);
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, 23);
    digitalWrite(latchPin, HIGH);
  }
  else if(dimPosition == 147)
  {
    digitalWrite(lastLED, LOW);
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, 46);
    digitalWrite(latchPin, HIGH);
  }
  else if(dimPosition == 156)
  {
    digitalWrite(lastLED, LOW);
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, 45);
    digitalWrite(latchPin, HIGH);
  }
  else if(dimPosition == 165)
  {
    digitalWrite(lastLED, LOW);
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, 43);
    digitalWrite(latchPin, HIGH);
  }
  else if(dimPosition == 174)
  {
    digitalWrite(lastLED, LOW);
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, 39);
    digitalWrite(latchPin, HIGH);
  }
  else if(dimPosition == 183)
  {
    digitalWrite(lastLED, LOW);
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, 78);
    digitalWrite(latchPin, HIGH);
  }
  else if(dimPosition == 192)
  {
    digitalWrite(lastLED, LOW);
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, 77);
    digitalWrite(latchPin, HIGH);
  }
  else if(dimPosition == 201)
  {
    digitalWrite(lastLED, LOW);
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, 75);
    digitalWrite(latchPin, HIGH);
  }
  else if(dimPosition == 210)
  {
    digitalWrite(lastLED, LOW);
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, 71);
    digitalWrite(latchPin, HIGH);
  }
  else if(dimPosition == 219)
  {
    digitalWrite(lastLED, LOW);
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, 142);
    digitalWrite(latchPin, HIGH);
  }
  else if(dimPosition == 228)
  {
    digitalWrite(lastLED, LOW);
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, 141);
    digitalWrite(latchPin, HIGH);
  }
  else if(dimPosition == 237)
  {
    digitalWrite(lastLED, LOW);
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, 139);
    digitalWrite(latchPin, HIGH);
  }
  else if(dimPosition == 246)
  {
    digitalWrite(lastLED, LOW);
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, 135);
    digitalWrite(latchPin, HIGH);
  }
  else if(dimPosition == 255)
  {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, 0);
    digitalWrite(latchPin, HIGH);
    digitalWrite(lastLED, HIGH);
  }
  else
  {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, 30);
    digitalWrite(latchPin, HIGH);
    digitalWrite(lastLED, LOW);
  }
  // Is someone pressing the rotary switch?
  reading_PinSW = digitalRead(PinSW);
    if (reading_PinSW == HIGH && previous_PinSW == LOW && millis() - time > debounce) {
      if (LOAD_status == HIGH)
        LOAD_status = LOW;
      else
        LOAD_status = HIGH;
  Serial.println(LOAD_status);
      time = millis();    
    }
  
    digitalWrite(SwitchOUT, LOAD_status);
  
    digitalWrite(Light, Light_status);
    previous_PinSW = reading_PinSW;
  

  // If the current rotary switch position has changed then update everything
  if (dimPosition != lastCount) {
    
    // Our LED gets brighter or dimmer
    analogWrite(SignalOUT, dimPosition);

    // Write out to serial monitor the value and direction
    Serial.print(dimPosition > lastCount ? "Up  :" : "Down:");
    Serial.println(dimPosition);

    // Keep track of this new value
    lastCount = dimPosition ;
  }


  if (irrecv.decode(&results)) {
    switch(results.value)
    {  
    case 0xFFA25D: 
      { 
        Serial.println(" ON/OFF"); 
        if (LOAD_status == HIGH)
          LOAD_status = LOW;
        else
          LOAD_status = HIGH;
      }
      break;  
    case 0xFF629D:  
      Serial.println(" Stop"); 
      break;  
    case 0xFFE21D:  
      Serial.println(" Mute"); 
      if (Light_status == HIGH)
          Light_status = LOW;
        else
          Light_status = HIGH;
      break;  
    case 0xFF22DD:  
      Serial.println(" Mode"); 
      break;  
    case 0xFF02FD:  
      Serial.println(" Back"); 
      break;  
    case 0xFFC23D:  
      Serial.println(" EQ"); 
      break;  
    case 0xFFE01F:  
      Serial.println(" PREV"); 
      break;  
    case 0xFFA857:  
      Serial.println(" NEXT"); 
      break;  
    case 0xFF906F:  
      Serial.println(" Play/Push"); 
      break;  
    case 0xFF6897:
      {
        dimPosition-=9 ; 
        // Restrict value from 0 to +255
        dimPosition = min(255, max(111, dimPosition));    
        Serial.println(" Vol-");  
      } 
      break;  
    case 0xFF9867:  
      {
        dimPosition+=9 ; 
        // Restrict value from 0 to +255
        dimPosition = min(255, max(111, dimPosition));    
        Serial.println(" Vol-");  
      }
      break;  
    case 0xFFB04F:
      {  
        dimPosition = 111;
        Serial.println(" 0"); 
      }
      break;  
    case 0xFF30CF:  
      {  
        dimPosition = 129;
        Serial.println(" 1"); 
      }
      break;  
    case 0xFF18E7:  
      {  
        dimPosition = 147;
        Serial.println(" 2"); 
      }
      break;  
    case 0xFF7A85:  
      {  
        dimPosition = 165;
        Serial.println(" 3"); 
      } 
      break;  
    case 0xFF10EF:  
      {  
        dimPosition = 183;
        Serial.println(" 4"); 
      } 
      break;  
    case 0xFF38C7:  
      {  
        dimPosition = 201;
        Serial.println(" 5"); 
      } 
      break;  
    case 0xFF5AA5:  
      {  
        dimPosition = 219;
        Serial.println(" 6"); 
      } 
      break;  
    case 0xFF42BD:  
      {  
        dimPosition = 237;
        Serial.println(" 7"); 
      }
      break;  
    case 0xFF4AB5:  
      {  
        dimPosition = 246;
        Serial.println(" 8"); 
      } 
      break;  
    case 0xFF52AD:  
      {  
        dimPosition = 255;
        Serial.println(" 9"); 
      } 
      break;  
    //default: 
      //Serial.println(" other button   ");  
    }
    irrecv.resume(); // Receive the next value
  }
}
