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

  
}
