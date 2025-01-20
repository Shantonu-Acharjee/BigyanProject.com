#include <IRremote.h>
#include <EEPROM.h>
//#include <UIPEthernet.h>
//#include "PubSubClient.h"
#define latchPin A1
#define clockPin A0
#define dataPin A2
#define lastLED 8
int IRPIN = 6;
// Used for generating interrupts using CLK signal
const int PinA = 3; 
// Used for reading DT signal
const int PinB = 4;
// Used for the push button switch
const int FanSW = 5; 
const int LightSW = 10; 
// Keep track of last rotary value
int LastSpeed = 50;
// Updated by the ISR (Interrupt Service Routine)
volatile int FanSpeed = 111;
int CurrentFanSW; 
int PreviousFanSW = HIGH;
int CurrentLightSW; 
int PreviousLightSW = HIGH;
long time = 0;         // the last time the output pin was toggled
long debounce = 200;   // the debounce time, increase if the output flickers
int SignalOUT = 9;
int Fan = 7;
int FanStatus = LOW;
int Light = 2;
int LightStatus = LOW;
int ELightStatus = 0;
int EFanStatus = 1;
int EFanSpeed = 2;
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
      FanSpeed+=9 ; // Could be -5 or -10
    }
    else {
      FanSpeed-=9 ; // Could be +5 or +10
    }
    // Restrict value from 0 to +255
    FanSpeed = min(255, max(111, FanSpeed));
    // Keep track of when we were here last (no more than every 5ms)
    lastInterruptTime = interruptTime;
  }
}

void setup()
{
  //Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
  // Rotary pulses are INPUTs
  pinMode(PinA, INPUT);
  pinMode(PinB, INPUT);
  // Switch is floating so use the in-built PULLUP so we don't need a resistor
  pinMode(FanSW, INPUT_PULLUP);
  pinMode(LightSW, INPUT_PULLUP);
  pinMode(Fan, OUTPUT);
  pinMode(Light, OUTPUT);
  pinMode(lastLED, OUTPUT);
  pinMode(SignalOUT, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
    // Attach the routine to service the interrupts
  attachInterrupt(digitalPinToInterrupt(PinA), isr, LOW);
  //Serial.println("Start");
  delay(1000);
  FanSpeed = EEPROM.read(EFanSpeed);
  FanStatus = EEPROM.read(EFanStatus);
  LightStatus = EEPROM.read(ELightStatus);
  digitalWrite(Fan, FanStatus);
  digitalWrite(Light, LightStatus);
}

void loop(){ 
  
  // Is someone pressing the rotary switch?
  CurrentFanSW = digitalRead(FanSW);
  if (CurrentFanSW == HIGH && PreviousFanSW == LOW && millis() - time > debounce) {
    if (FanStatus == HIGH)
      FanStatus = LOW;
    else
      FanStatus = HIGH;
    time = millis(); 
    digitalWrite(Fan, FanStatus); 
    EEPROM.update(EFanStatus, FanStatus); 
  } 
  PreviousFanSW = CurrentFanSW;  
  
  CurrentLightSW = digitalRead(LightSW);
  if (CurrentLightSW == HIGH && PreviousLightSW == LOW && millis() - time > debounce) {
    if (LightStatus == HIGH)
      LightStatus = LOW;
    else
      LightStatus = HIGH;
    time = millis(); 
    digitalWrite(Light, LightStatus); 
    EEPROM.update(ELightStatus, LightStatus);  
  } 
  PreviousLightSW = CurrentLightSW;  

  // If the current rotary switch position has changed then update everything
  if (FanSpeed != LastSpeed) {
    if(FanSpeed == 111)
    {
      digitalWrite(lastLED, LOW);
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, LSBFIRST, 30);
      digitalWrite(latchPin, HIGH);
    }
    else if(FanSpeed == 120)
    {
      digitalWrite(lastLED, LOW);
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, LSBFIRST, 29);
      digitalWrite(latchPin, HIGH);
    }
    else if(FanSpeed == 129)
    {
      digitalWrite(lastLED, LOW);
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, LSBFIRST, 27);
      digitalWrite(latchPin, HIGH);
    }
    else if(FanSpeed == 138)
    {
      digitalWrite(lastLED, LOW);
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, LSBFIRST, 23);
      digitalWrite(latchPin, HIGH);
    }
    else if(FanSpeed == 147)
    {
      digitalWrite(lastLED, LOW);
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, LSBFIRST, 46);
      digitalWrite(latchPin, HIGH);
    }
    else if(FanSpeed == 156)
    {
      digitalWrite(lastLED, LOW);
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, LSBFIRST, 45);
      digitalWrite(latchPin, HIGH);
    }
    else if(FanSpeed == 165)
    {
      digitalWrite(lastLED, LOW);
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, LSBFIRST, 43);
      digitalWrite(latchPin, HIGH);
    }
    else if(FanSpeed == 174)
    {
      digitalWrite(lastLED, LOW);
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, LSBFIRST, 39);
      digitalWrite(latchPin, HIGH);
    }
    else if(FanSpeed == 183)
    {
      digitalWrite(lastLED, LOW);
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, LSBFIRST, 78);
      digitalWrite(latchPin, HIGH);
    }
    else if(FanSpeed == 192)
    {
      digitalWrite(lastLED, LOW);
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, LSBFIRST, 77);
      digitalWrite(latchPin, HIGH);
    }
    else if(FanSpeed == 201)
    {
      digitalWrite(lastLED, LOW);
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, LSBFIRST, 75);
      digitalWrite(latchPin, HIGH);
    }
    else if(FanSpeed == 210)
    {
      digitalWrite(lastLED, LOW);
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, LSBFIRST, 71);
      digitalWrite(latchPin, HIGH);
    }
    else if(FanSpeed == 219)
    {
      digitalWrite(lastLED, LOW);
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, LSBFIRST, 142);
      digitalWrite(latchPin, HIGH);
    }
    else if(FanSpeed == 228)
    {
      digitalWrite(lastLED, LOW);
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, LSBFIRST, 141);
      digitalWrite(latchPin, HIGH);
    }
    else if(FanSpeed == 237)
    {
      digitalWrite(lastLED, LOW);
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, LSBFIRST, 139);
      digitalWrite(latchPin, HIGH);
    }
    else if(FanSpeed == 246)
    {
      digitalWrite(lastLED, LOW);
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, LSBFIRST, 135);
      digitalWrite(latchPin, HIGH);
    }
    else if(FanSpeed == 255)
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
    // Our LED gets brighter or dimmer
    analogWrite(SignalOUT, FanSpeed);

    // Keep track of this new value
    LastSpeed = FanSpeed ;
    EEPROM.update(EFanSpeed, FanSpeed);
  }


  if (irrecv.decode(&results)) {
    switch(results.value)
    {  
    case 0xFFA25D: 
      { 
        //Serial.println(" ON/OFF"); 
        if (FanStatus == HIGH)
          FanStatus = LOW;
        else
          FanStatus = HIGH;
      }  
      digitalWrite(Fan, FanStatus); 
      EEPROM.update(EFanStatus, FanStatus);
      break;  
    case 0xFF629D:  
      //Serial.println(" Stop"); 
      break;  
    case 0xFFE21D:  
      //Serial.println(" Mute"); 
      if (LightStatus == HIGH)
          LightStatus = LOW;
      else
          LightStatus = HIGH;
      digitalWrite(Light, LightStatus);
      EEPROM.update(ELightStatus, LightStatus);
      break;  
    case 0xFF22DD:  
      //Serial.println(" Mode"); 
      //Serial.println(EEPROM.read(EFanStatus)); 
      break;  
    case 0xFF02FD:  
      //Serial.println(" Back"); 
      break;  
    case 0xFFC23D:  
      //Serial.println(" EQ"); 
      break;  
    case 0xFFE01F:  
      //Serial.println(" PREV"); 
      break;  
    case 0xFFA857:  
      //Serial.println(" NEXT"); 
      break;  
    case 0xFF906F:  
      //Serial.println(" Play/Push"); 
      break;  
    case 0xFF6897:
      {
        FanSpeed-=9 ; 
        // Restrict value from 0 to +255
        FanSpeed = min(255, max(111, FanSpeed));    
        //Serial.println(" Vol-");  
      } 
      break;  
    case 0xFF9867:  
      {
        FanSpeed+=9 ; 
        // Restrict value from 0 to +255
        FanSpeed = min(255, max(111, FanSpeed));    
        //Serial.println(" Vol-");  
      }
      break;  
    case 0xFFB04F:
      {  
        FanSpeed = 111;
        //Serial.println(" 0"); 
      }
      break;  
    case 0xFF30CF:  
      {  
        FanSpeed = 129;
        //Serial.println(" 1"); 
      }
      break;  
    case 0xFF18E7:  
      {  
        FanSpeed = 147;
        //Serial.println(" 2"); 
      }
      break;  
    case 0xFF7A85:  
      {  
        FanSpeed = 165;
        //Serial.println(" 3"); 
      } 
      break;  
    case 0xFF10EF:  
      {  
        FanSpeed = 183;
        //Serial.println(" 4"); 
      } 
      break;  
    case 0xFF38C7:  
      {  
        FanSpeed = 201;
        //Serial.println(" 5"); 
      } 
      break;  
    case 0xFF5AA5:  
      {  
        FanSpeed = 219;
        //Serial.println(" 6"); 
      } 
      break;  
    case 0xFF42BD:  
      {  
        FanSpeed = 237;
        //Serial.println(" 7"); 
      }
      break;  
    case 0xFF4AB5:  
      {  
        FanSpeed = 246;
        //Serial.println(" 8"); 
      } 
      break;  
    case 0xFF52AD:  
      {  
        FanSpeed = 255;
        //Serial.println(" 9"); 
      } 
      break;  
    //default: 
      //Serial.println(" other button   ");  
    }
    irrecv.resume(); // Receive the next value
  }
}
