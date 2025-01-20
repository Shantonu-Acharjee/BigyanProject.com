
/*---------------------------
IR LED and phototransistor beam test and calibration

debug stuff:
min/max val = whatever the value is when it changes
LED is always on
----------------------------*/
// defines for pins
#define IRinputAnalogPin A0   // analog input pin
#define LEDsignalPin 13      // LED signal pin

// int and long values
int val = 0;           // variable to store the value read from input pin
int minVal = 1000;
int maxVal = 0;
int oldVal= 0;

// strings
String outputString;
String intro = "JTD IR test and calibration init";
boolean needIntro = true;
String signalBase = "Signal Strength: ";
String minBase = "Min: ";
String maxBase = "Max: ";

void setup()
{
  pinMode(LEDsignalPin, OUTPUT);
  Serial.begin(9600);
}
void loop()
{


// read value of analog input   
  val = analogRead(IRinputAnalogPin);    // read the input pin
  Serial.println(val);             // debug value
  if(val <850)
  digitalWrite(LEDsignalPin, HIGH);    // turn signal LED on
  else
  digitalWrite(LEDsignalPin, LOW);    // turn signal LED on
  
}
