#include <IRremote.h>
int RECV_PIN = 8;

IRrecv irrecv(RECV_PIN);

decode_results results;
int AC_LOAD = 9;
int LOAD_status = HIGH;
int dimming = 128;
int temp = 0;
int numEnc = 2;
int encPinA = 4;
int encPinB = 5;
int lastModeA;
int lastModeB;
int curModeA;
int curModeB;
int dimtime;
int encPos;
int encPosLast;
int change = 0;
int c = 0;
int d = 0;
unsigned long currentTime;
unsigned long loopTime;   
int reading;           
int previous = LOW;
int switchin = 6;
int switchout = 7;
void setup()
{
  Serial.begin(9600);
  attachInterrupt(0, zero_crosss_int, RISING);
  irrecv.enableIRIn(); // Start the receiver
  pinMode(AC_LOAD, OUTPUT);
  pinMode(encPinA, INPUT);
  pinMode(encPinB, INPUT);
  pinMode(switchin, INPUT);
  pinMode(switchout, OUTPUT);
  digitalWrite(switchin, HIGH);
  lastModeA = LOW;
  lastModeB = LOW;
  curModeA = LOW;
  curModeB = LOW;
  encPos = 0;
  encPosLast = 0;
}

void zero_crosss_int()
{
  dimtime = (75 * dimming);
  delayMicroseconds(dimtime);
  digitalWrite(AC_LOAD, HIGH);
  delayMicroseconds(10);
  digitalWrite(AC_LOAD, LOW);
}

void loop()  {
  
  change = 0;
  currentTime = millis();
  if ((currentTime >= (loopTime + 5)) && (LOAD_status == 1)) {
    curModeA = digitalRead(encPinA);
    curModeB = digitalRead(encPinB);
    if ((!curModeA) && (lastModeA)) {
      change = 1;
      if (curModeB) {
        if (encPos + 1 <= 110) {
          encPos = encPos + 5;
          dimming = encPos;
        }
      }
      else {
        if (encPos - 1 >= 0)
        {
          encPos = encPos - 5;
          dimming = encPos;
        }
      }
    }
    lastModeA = curModeA;
    loopTime = currentTime;
  }
    reading = digitalRead(switchin);
  if (reading == HIGH && previous == LOW) {
    if (LOAD_status == HIGH)
      LOAD_status = LOW;
    else
      LOAD_status = HIGH;   
  }

  digitalWrite(switchout, LOAD_status);

  previous = reading;


  if (irrecv.decode(&results)) {
    switch(results.value)
    {  
    case 0xFFA25D:  
      Serial.println(" ON/OFF"); 
      break;  
    case 0xFF629D:  
      Serial.println(" Stop"); 
      break;  
    case 0xFFE21D:  
      Serial.println(" Mute"); 
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
      Serial.println(" Vol-"); 
      break;  
    case 0xFF9867:  
      Serial.println(" Vol+"); 
      break;  
    case 0xFFB04F:  
      Serial.println(" 0"); 
      break;  
    case 0xFF30CF:  
      Serial.println(" 1              "); 
      break;  
    case 0xFF18E7:  
      Serial.println(" 2              "); 
      break;  
    case 0xFF7A85:  
      Serial.println(" 3              "); 
      break;  
    case 0xFF10EF:  
      Serial.println(" 4              "); 
      break;  
    case 0xFF38C7:  
      Serial.println(" 5              "); 
      break;  
    case 0xFF5AA5:  
      Serial.println(" 6              "); 
      break;  
    case 0xFF42BD:  
      Serial.println(" 7              "); 
      break;  
    case 0xFF4AB5:  
      Serial.println(" 8              "); 
      break;  
    case 0xFF52AD:  
      Serial.println(" 9              "); 
      break;  
    //default: 
      //Serial.println(" other button   ");  
    }
    irrecv.resume(); // Receive the next value
  }
}
