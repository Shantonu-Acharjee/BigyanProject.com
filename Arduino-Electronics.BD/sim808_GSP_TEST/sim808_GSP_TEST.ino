#include "DHT.h"
#include <SoftwareSerial.h>
//#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN 2
#define RAINPIN A0
#define LDRPIN 4
#define DHTTYPE DHT11
LiquidCrystal_I2C lcd(0x27,16,2);
DHT dht(DHTPIN, DHTTYPE);
//StaticJsonBuffer<200> jsonBuffer;
#define DEBUG true
#define REPORTING_PERIOD_MS     1000
SoftwareSerial sim808(6, 7);
uint32_t tsLastReport = 0;
char phone_no[] = "+8801983715331"; 
String r_data[5];
String state,timegps,latitude,longitude,speed;
String apn = "GPINTERNET";                    //APN
String apn_u = "";                     //APN-Username
String apn_p = "";                     //APN-Password
String url = "http://123.200.2.189/kkh/api/Weather/Add";  //URL of Server

float h;
float t; 
int RAIN = 0;
int LDR = 0;
void setup() {

  Serial.begin(9600);
  pinMode(LDRPIN,INPUT);
  lcd.init();
  lcd.backlight();
  dht.begin();
  lcd.setCursor(0,0);
  lcd.print(" Welcome  ");
  sim808.begin(9600);
  //delay(15000);
  while (sim808.available()) {
    Serial.write(sim808.read());
  }
  //DynamicJsonBuffer jsonBuffer;
  delay(2000);
  
}

void loop() {  
  lcd.clear();
  lcd.setCursor(0,0);
  h = dht.readHumidity();
  t = dht.readTemperature(); 
  RAIN = analogRead(RAINPIN);
  LDR = digitalRead(LDRPIN);
  
  lcd.print("T:");
  lcd.print(t);
  lcd.print("H:");
  lcd.print(h);
  lcd.setCursor(0,1);
  lcd.print("La:");
  lcd.print(latitude);
  lcd.print("Lo:");
  lcd.print(longitude);
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(RAIN);
  Serial.print(F("="));  
  Serial.println(LDR); 
  //gps_test();
  gsm_http_post();
  delay(60000); 
}

void gsm_http_post() {
  gsm_config_gprs();
  Serial.println(" --- Start GPRS & HTTP --- ");
  gsm_send_serial("AT+SAPBR=1,1");
  gsm_send_serial("AT+SAPBR=2,1");
  gsm_send_serial("AT+HTTPINIT");
  gsm_send_serial("AT+HTTPPARA=CID,1");
  
  //StaticJsonBuffer<200> jsonBuffer;
  //JsonObject& object = jsonBuffer.createObject();
  
  //object.set("macAddress","5244-5874-1254-9425");
  //object.set("surfaceTemperature",t);
  //object.set("relativeHumidity",h);
  //object.set("rainfall",RAIN);
  //object.set("latitude",latitude);
  //object.set("longitude",longitude);
  //object.set("velocity",0);
  //object.printTo(Serial);
  //Serial.println(" ");  
  String sendtoserver = "macAddress=5244-5874-1254-9425&surfaceTemperature="+String(t)+"&relativeHumidity="+String(h)+"&latitude="+String(latitude)+"&longitude="+String(longitude);
  //object.prettyPrintTo(sendtoserver);

  
  gsm_send_serial("AT+HTTPPARA=URL," + url);
  //gsm_send_serial("AT+HTTPPARA=CONTENT,application/json");
  gsm_send_serial("AT+HTTPDATA=" + String(sendtoserver.length()) + ",5000");
  gsm_send_serial(sendtoserver);
  gsm_send_serial("AT+HTTPACTION=1");
  gsm_send_serial("AT+HTTPREAD");
  gsm_send_serial("AT+HTTPTERM");
  gsm_send_serial("AT+SAPBR=0,1");
}
void gsm_config_gprs() {
  Serial.println(" --- CONFIG GPRS --- ");
  gsm_send_serial("AT+SAPBR=3,1,Contype,GPRS");
  gsm_send_serial("AT+SAPBR=3,1,APN," + apn);
  if (apn_u != "") {
    gsm_send_serial("AT+SAPBR=3,1,USER," + apn_u);
  }
  if (apn_p != "") {
    gsm_send_serial("AT+SAPBR=3,1,PWD," + apn_p);
  }
}
void gsm_send_serial(String command) {
  Serial.println("Send ->: " + command);
  sim808.println(command);
  long wtimer = millis();
  while (wtimer + 3000 > millis()) {
    while (sim808.available()) {
      Serial.write(sim808.read());
    }
  }
  Serial.println();
}
void gps_test() {
  
  sim808.println("AT+CSMP=17,167,0,0");
  delay(100);
  sim808.println("AT+CMGF=1");
  delay(400);
  
  sendData("AT+CGNSPWR=1",1000,DEBUG);
  delay(50);
  sendData("AT+CGNSSEQ=RMC",1000,DEBUG);
  delay(150);
  sendTabData("AT+CGNSINF",1000,DEBUG);
  delay(150);
  sendTabData("AT+CGNSINF",1000,DEBUG);
  if (state !=4) {
    Serial.println("State  :"+state);
    Serial.println("Time  :"+timegps);
    Serial.println("Latitude  :"+latitude);
    Serial.println("Longitude  :"+longitude);
    
    sim808.print("AT+CMGS=\"");
    sim808.print(phone_no);
    sim808.println("\"");
    
    delay(300);
    
    sim808.print("Location: http://maps.google.com/maps?q=loc:");
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
    speed =r_data[6];
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
