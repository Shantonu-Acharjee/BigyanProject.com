#include <SoftwareSerial.h>
#include "MAX30100_PulseOximeter.h"
#define DEBUG true
#define REPORTING_PERIOD_MS     1000
SoftwareSerial sim808(7,6);
PulseOximeter pox;
uint32_t tsLastReport = 0;
char police_phone_no[] = "+8801759211397"; 
char medical_phone_no[] = "+8801682715386";
String r_data[5];
String state,timegps,latitude,longitude;
int alcohol_sensor=8;
int motor_pin=9;
float bmp;

void setup() {
  sim808.begin(9600);
  Serial.begin(9600);

  pinMode(motor_pin, OUTPUT);
  pinMode(alcohol_sensor, INPUT);
  delay(50);
  
  sim808.println("AT+CSMP=17,167,0,0");
  delay(100);
  sim808.println("AT+CMGF=1");
  delay(400);
  
  sendData("AT+CGNSPWR=1",1000,DEBUG);
  delay(50);
  sendData("AT+CGNSSEQ=RMC",1000,DEBUG);
  delay(150);
  sendTabData("AT+CGNSINF",1000,DEBUG);
  Serial.println("Initializing pulse oximeter..");
  if (!pox.begin()) {
      Serial.println("FAILED");
      for(;;);
  } else {
      Serial.println("SUCCESS");
  }
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
  pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop() {  
    pox.update();
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        bmp = pox.getHeartRate();
        if (!digitalRead(alcohol_sensor))
        {
          Serial.println("Driver Alcoholic");
          Serial.println("Engine Stop");
          digitalWrite(motor_pin, LOW);
          notify_police();
          delay(20000);  
        }
        else
        {
          Serial.println("Driver Not Alcoholic"); 
          Serial.print("Heart rate:");
          Serial.print(bmp);
          Serial.println("bpm");  
          if(bmp > 72)
          {
            Serial.println("Heart rate Abnormal");
            Serial.println("Engine Stop");
            digitalWrite(motor_pin, LOW);
            notify_medical(); 
            delay(200000);  
          }
          else{
            Serial.println("Heart rate Normal");
            Serial.println("Engine Running..");
            digitalWrite(motor_pin, HIGH);
          }
        }
        Serial.println("GPS Location:");
        Serial.println("State  :"+state);
        Serial.println("Latitude  :"+latitude);
        Serial.println("Longitude  :"+longitude);
        tsLastReport = millis();
    }
}

void onBeatDetected()
{
    Serial.println("Beat!");
}

void notify_police() {
  sendTabData("AT+CGNSINF",1000,DEBUG);
  if (state !=0) {
    Serial.println("State  :"+state);
    Serial.println("Time  :"+timegps);
    Serial.println("Latitude  :"+latitude);
    Serial.println("Longitude  :"+longitude);
    
    sim808.print("AT+CMGS=\"");
    sim808.print(police_phone_no);
    sim808.println("\"");
    
    delay(300);
    
    sim808.print("Alcoholic Driver Detected! Location: http://maps.google.com/maps?q=loc:");
    sim808.print(latitude);
    sim808.print(",");
    sim808.print (longitude);
    delay(200);
    sim808.println((char)26); // End AT command with a ^Z, ASCII code 26
    delay(200);
    sim808.println();
    delay(20000);
    sim808.flush();
  
  } else {
    Serial.println("GPS Initializing…");
  }
}
void notify_medical() {
  sendTabData("AT+CGNSINF",1000,DEBUG);
  if (state !=0) {
    Serial.println("State  :"+state);
    Serial.println("Time  :"+timegps);
    Serial.println("Latitude  :"+latitude);
    Serial.println("Longitude  :"+longitude);
    
    sim808.print("AT+CMGS=\"");
    sim808.print(medical_phone_no);
    sim808.println("\"");
    
    delay(300);
    
    sim808.print("Patient BPM:");
    sim808.print(bmp);
    sim808.print(" Heart Patient Detected! Location: http://maps.google.com/maps?q=loc:");
    sim808.print(latitude);
    sim808.print(",");
    sim808.print (longitude);
    delay(200);
    sim808.println((char)26); // End AT command with a ^Z, ASCII code 26
    delay(200);
    sim808.println();
    delay(20000);
    sim808.flush();
  
  } else {
    Serial.println("GPS Initializing…");
  }
}

void sendTabData(String command , const int timeout , boolean debug){

  sim808.println(command);
  long int time = millis();
  int i = 0;
  
  while((time+timeout) > millis()){
    while(sim808.available()){
      char c = sim808.read();
      if (c != ',') {
        r_data[i] +=c;
        delay(100);
      } else {
        i++;
      }
      if (i == 5) {
        delay(100);
        goto exitL;
      }
    }
  }exitL:
  if (debug == true && latitude == "") {
    state = r_data[1];
    timegps = r_data[2];
    latitude = r_data[3];
    longitude =r_data[4];
  }
}
String sendData (String command , const int timeout ,boolean debug){
  String response = "";
  sim808.println(command);
  long int time = millis();
  int i = 0;
  
  while ( (time+timeout ) > millis()){
    while (sim808.available()){
      char c = sim808.read();
      response +=c;
    }
  }
  if (debug) {
    Serial.print(response);
  }
  return response;
}
