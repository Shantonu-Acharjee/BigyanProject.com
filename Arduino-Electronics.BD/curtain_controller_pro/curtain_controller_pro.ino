//#include <AccelStepper.h>
//#define motorInterfaceType 1
const int DIR = 2;
const int STEP = 3;
const int EndStopL = 4;
const int EndStopR = 5;
const int OpenPin = 6;
const int ClosePin = 7;
const int NOpenPin = 8;
const int NClosePin = 9;
const int ENPin = 13;

boolean OpenStatus = false;
boolean CloseStatus = false;
//AccelStepper stepper = AccelStepper(motorInterfaceType, STEP, DIR);

void setup()
{
  Serial.begin(115200);
  delay(100);
  pinMode(STEP, OUTPUT);
  pinMode(DIR, OUTPUT);
  pinMode(EndStopL, INPUT_PULLUP);
  pinMode(EndStopR, INPUT_PULLUP);
  pinMode(OpenPin, INPUT_PULLUP);
  pinMode(ClosePin, INPUT_PULLUP);
  pinMode(NOpenPin, OUTPUT);
  pinMode(NClosePin, OUTPUT);
  pinMode(ENPin, OUTPUT);
  digitalWrite(ENPin,HIGH);
  //stepper.setMaxSpeed(2000);
}

void loop() {
  if(digitalRead(OpenPin) == HIGH && digitalRead(ClosePin) == LOW){
    digitalWrite(ENPin,LOW);
    OpenStatus = true;
    CloseStatus = false;
  }
  else if(digitalRead(OpenPin) == LOW && digitalRead(ClosePin) == HIGH){
    digitalWrite(ENPin,LOW);
    OpenStatus = false;
    CloseStatus = true;
  }
  else{
    digitalWrite(ENPin,HIGH);
    OpenStatus = false;
    CloseStatus = false;
  }
  if(OpenStatus == true){
    CloseStatus = false;
    OpenCurtain();
  }
  if(CloseStatus == true){
    OpenStatus = false;
    CloseCurtain();
  }
}

void OpenCurtain(){
  if(digitalRead(EndStopL) == LOW){
    digitalWrite(NOpenPin,HIGH);
    digitalWrite(DIR,HIGH);
    digitalWrite(STEP,HIGH); 
    delayMicroseconds(300); 
    digitalWrite(STEP,LOW); 
    delayMicroseconds(300); 
  }
  else{
    digitalWrite(NOpenPin,LOW); 
  }
}

void CloseCurtain(){
  if(digitalRead(EndStopR) == LOW){
    digitalWrite(NClosePin,HIGH);    
    digitalWrite(DIR,LOW);
    digitalWrite(STEP,HIGH); 
    delayMicroseconds(300); 
    digitalWrite(STEP,LOW); 
    delayMicroseconds(300); 
  } 
  else{
    digitalWrite(NClosePin,LOW);  
  }
}
