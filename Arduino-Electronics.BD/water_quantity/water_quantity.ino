#include <SoftwareSerial.h>
SoftwareSerial mySerial(12, 13); // RX, TX
#include "U8glib.h"  // Include U8glib library
#include "HX710B.h"
const int DOUT = 5;   //sensor data pin
const int SCK1  = 6;   //sensor clock pin
 
HX710B pressure_sensor; 
// Create display and set pins:
U8GLIB_PCD8544 u8g(52, 51, 45, 47, 49);   // (CLK=13, DIN=11, CE=10, DC=9, RST=8)

// defines pins numbers
const int trigPin = 9;
const int echoPin = 10;
const long REPORTING_PERIOD_MS = 5000;    // the debounce time; increase if the output flickers

// defines variables
long tsLastDisplay = 0;
long tsLastReport = 0;  // the last time the output pin was toggled
long duration;

char incomingChar;
String Data_SMS;
int flag = 0;

volatile int flow_frequency;
float l_minute;
unsigned char flowsensor = 2; // Sensor Input
unsigned long currentTime;
unsigned long cloopTime;

String ph = "0";
String color = "0";
String spe = "0";
String temp = "0";
String pressure = "0";
String flow = "0.0";
String volume = "0.0";
String str = "";


void f_flow() // Interrupt function
{
   flow_frequency++;
}

void setup() {
  Serial.begin(115200); // Starts the serial communication
  Serial1.begin(115200); // Starts the serial communication
  mySerial.begin(115200);
  pressure_sensor.begin(DOUT, SCK1);
  u8g.setFont(u8g_font_6x10);
  pinMode(8, OUTPUT); // Sets the trigPin as an Output
  digitalWrite(8, HIGH);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(11, OUTPUT); // Sets the trigPin as an Output
  digitalWrite(11, LOW);
  pinMode(3, OUTPUT); // Sets the trigPin as an Output
  digitalWrite(3, LOW);
  pinMode(4, OUTPUT); // Sets the trigPin as an Output
  digitalWrite(4, HIGH);
  pinMode(flowsensor, INPUT);
  digitalWrite(flowsensor, HIGH); // Optional Internal Pull-Up
  ReceiveMode();

  u8g.firstPage(); // Beginning of the picture loop
  do  // Include all you want to show on the display:
  {
    u8g.drawStr(10, 10, "  WELCOME");
    u8g.drawStr(35, 24, "TO");
    u8g.drawStr(4, 38, "WATER PROJECT");
       
  } while(u8g.nextPage());  // End of the picture loop  
  delay(2000);
  
  attachInterrupt(digitalPinToInterrupt(flowsensor), f_flow, RISING); // Setup Interrupt
}

void loop() {
  while(mySerial.available()>0){ 
    if(flag == 1)
    break;
    incomingChar=mySerial.read();
    Serial.print(incomingChar);
    if(incomingChar=='S') {
        flag = 1;
        //delay(10);
        Serial.print("...Request Received \n");
        Data_SMS = "FLOW = "+flow+" L/M"+" \nVOLUME ="+volume+" L"+"\nPRESSURE ="+pressure+" PSI";
        send_sms();
        ReceiveMode();
        incomingChar = 'F';
        mySerial.flush();
        delay(5000);
    }
   }
   
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculating the volume 
  int vol= duration*0.034/2;
  if(vol >= 30){
    vol = 0;
  }
  volume= map(vol, 0, 30, 30, 0);
  pressure = pressure_sensor.psi();
  if (millis() - tsLastDisplay > REPORTING_PERIOD_MS/10) {
    if(flow_frequency != 0){
      // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
      l_minute = (flow_frequency / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
      
      l_minute = l_minute/60;
      flow = l_minute;
      //spe = volume +l_minute;
      flow_frequency = 0; // Reset Counter
      Serial.print(l_minute, DEC); // Print litres/hour
      Serial.println(" L/M");
    }
    else {
      flow = flow_frequency;
    }
    str =String(ph)+String("=")+String(color)+String("=")+String(spe)+String("=")+String(temp)+String("=")+String(pressure)+String("=")+String(flow)+String("=")+String(volume);
    Serial.print("Values: ");
    Serial.println(str);
    u8g.setFont(u8g_font_5x7);
    u8g.firstPage(); // Beginning of the picture loop
    do  // Include all you want to show on the display:
    {
      u8g.drawStr(0, 10, "FLOW:");
      u8g.setPrintPos(46, 10);  // (x,y)
      u8g.print(flow);      
      u8g.drawStr(68, 10, "L/M");
      u8g.drawStr(0, 24, "VOLUME:");
      u8g.setPrintPos(46, 24);  // (x,y)
      u8g.print(volume);
      u8g.drawStr(68, 24, "L");
      u8g.drawStr(0, 38, "PRESSURE:");
      u8g.setPrintPos(46, 38);  // (x,y)
      u8g.print(pressure);
      u8g.drawStr(68, 38, "PSI");
         
    } while(u8g.nextPage());  // End of the picture loop  
    tsLastDisplay = millis();
  }
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    str =String(ph)+String("=")+String(color)+String("=")+String(spe)+String("=")+String(temp)+String("=")+String(pressure)+String("=")+String(flow)+String("=")+String(volume);
    Serial1.println(str);
    Serial.println("Json Sent");
    tsLastReport = millis();
  }
}
void clearOLED(){
  u8g.firstPage();
  do {
    
  } while( u8g.nextPage() );
}
void send_sms()
{  
  mySerial.println("AT");
  updateSerial();

  mySerial.println("AT+CMGF=1");
  updateSerial();
  mySerial.println("AT+CMGS=\"+8801983715331\"");
  updateSerial();
  mySerial.print(Data_SMS);
  updateSerial();
  mySerial.write(26);
}
void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());
  }
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());
  }
}
void ReceiveMode(){       //Set the SIM800L Receive mode
  
  mySerial.println("AT"); //If everything is Okay it will show "OK" on the serial monitor
  updateSerial();
  mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  mySerial.println("AT+CNMI=2,2,0,0,0"); //Configure the SIM800L on how to manage the Received SMS... Check the SIM800L AT commands manual
  updateSerial();
}
