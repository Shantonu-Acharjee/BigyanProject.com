/*  Controlling a 4 wire computer fan via PWM and potmeter.
Hardware  : 1 * Arduino UNO R3
          : 1 * 4 Wire fan TA450DC or TA350DC in this case. Ex DELL PC.
          : 1 * 12 volt DC 1,5 Amp. external powersource for the fan.
          : 1 * Potentiometer of 1 KOhm.
          
WireColor Coding of the fan : black  = -12v 
                            : red    = +12v 
                            : yellow or white = Sensor 
                            : Blue   = PWM
                            
This sketch is in the public domain, no copyright at all.
JayBrad 13/07/2015 The Netherlands
*/


float fanSpeed   = 0;  // A0 op de Arduino / Loper van de Potmeter
float fanPWM     = 3;  // A3 op de Arduino. / Instellen van toerental 0/255
int   fanSensor  = 8;  // Pin D8 op de Arduino / Uitlezen van toerental 0/1023
int   Puls       = 2;  // Hier het aantal pulsen per omwenteling van de fan. Te vinden in specs van de fan.   

int Speed = 0; // Var die nodig is om de snelheid in te stellen.

unsigned long SensorPulsTijd;

void setup()
 {
 Serial.begin(9600);
 pinMode(fanSensor, INPUT);
 pinMode(fanSpeed, INPUT);
 digitalWrite(fanSensor,HIGH);

 }

void loop()
 {
 Speed = analogRead(fanSpeed)/4; // analogRead waarde = 0-1023 en analogWrite waarde = 0-255 dus moeten we eerst door 4 delen om op de goede waarde te komen.
 analogWrite(fanPWM, Speed);          // analogWrite waarde = van 0 tot 255

 SensorPulsTijd = pulseIn(fanSensor, LOW);
 double frequency = 1000000/SensorPulsTijd;  // 1000000 microsec / SensorPulsTijd = aantal microsec per puls.

 Serial.print("Pulsduur in microsec.     = ");
 Serial.println(SensorPulsTijd);
 Serial.print("Rotatietijd in microsec.  = ");
 Serial.println(SensorPulsTijd*Puls);
 Serial.print("Frequentie in Herz        = ");
 Serial.println(frequency/Puls, 0);
 Serial.print("Toerental per Minuut      = ");
 Serial.println(frequency/Puls*60);
 Serial.println();
 Serial.println();
 delay(2000);

 }
