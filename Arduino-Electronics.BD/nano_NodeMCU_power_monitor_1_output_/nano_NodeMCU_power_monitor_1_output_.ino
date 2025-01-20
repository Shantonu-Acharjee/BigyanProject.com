#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <ArduinoJson.h>
SoftwareSerial nodeMcu(10,11); // (Rx, Tx)
//SoftwareSerial SIM800(13, 12);
const int vo = 2, rs = 3, en = 4, d4 = 5, d5 = 6, d6 = 7, d7 = 8;
const int sensor1 = 46, sensor2 = 44, sensor3 = 42, sensor4 = 40, sensor5 = 38, sensor6 = 36;
const int post1 = 45, post2 = 43, post3 = 41, post4 = 39, post5 = 37;
int flag1 = 0, flag2 = 0, flag3 = 0, flag4 = 0, SMSflag = 0, SMSflag1 = 0;
unsigned long previousMillis1 = 0, previousMillis2 = 0, previousMillis3 = 0, previousMillis4 = 0, previousMillis5 = 0, previousMillis6 = 0;
const int voltageSensor = A0;
const int lightSensor = A7;
const int rainSensor = 26;
float R1 = 30000.0;
float R2 = 7500.0;
const int power = 47;
const int powerSwitch = 9;
const long interval = 5000;
String str;
float msec=0, last_msec=0, elasped_msec=0, elasped_time=0, ampSecs = 0, ampHours=0, watts=0, wattSecs = 0, wattHours=0;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
void setup(){
 Serial.begin(9600);
 nodeMcu.begin(9600);
 Serial1.begin(9600); 
 pinMode(vo,OUTPUT);
 pinMode(post1,OUTPUT);
 pinMode(post2,OUTPUT);
 pinMode(post3,OUTPUT);
 pinMode(post4,OUTPUT);
 pinMode(post5,OUTPUT);
 pinMode(power,OUTPUT);
 pinMode(powerSwitch,INPUT);
 pinMode(rainSensor,INPUT);
 pinMode(sensor1,INPUT);
 pinMode(sensor2,INPUT);
 pinMode(sensor3,INPUT);
 pinMode(sensor4,INPUT);
 pinMode(sensor5,INPUT);
 pinMode(sensor6,INPUT);
 digitalWrite(post1,HIGH);
 digitalWrite(post2,HIGH);
 digitalWrite(post3,HIGH);
 digitalWrite(post4,HIGH);
 digitalWrite(post5,HIGH);
 digitalWrite(power,HIGH);
 analogWrite(vo, 100);
 delay(1000);
 lcd.begin(16, 2);
 lcd.clear();
 lcd.setCursor(0, 0);
 lcd.print("Initializing......");
 delay(15000);
 //send_sms();
}
void loop()
{
  int LightSensorValue = analogRead(lightSensor);
  int RainSensorValue = digitalRead(rainSensor);
  if(LightSensorValue < 200 && digitalRead(powerSwitch) == HIGH){
    digitalWrite(power,LOW);
    if(SMSflag == 0 && analogRead(A8) > 1000)
    {
      send_sms();
      SMSflag = 1;
    }
  }
  else
  {
    SMSflag = 0;
    digitalWrite(power,HIGH);
  }
  if(SMSflag1 == 0 && RainSensorValue == 1){
    send_sms1();
    SMSflag1 = 1;
  }
  float AcsValue=0.0,Samples=0.0,AvgAcs=0.0,currentValue=0.0;
  //5V = ADC value 1024 => 1 ADC value = (5/1024)Volt= 0.0048828Volt
  //Vout=Vin*R2/(R1+R2) => Vin = Vout*(R1+R2)/R2   R1=30k and R2=7.5k
  for (int x = 0; x < 150; x++){ //Get 150 samples
    AcsValue = analogRead(A1);     //Read current sensor values   
    Samples = Samples + AcsValue;  //Add samples together
    delay (3); // let ADC settle before next sample 3ms
  }
  AvgAcs=Samples/150.0;
  AcsValue=(AvgAcs * 5.0) / 1024.0;
  float voltageValue = AcsValue / (R2/(R1+R2));
  Samples=0.0;
  for (int x = 0; x < 150; x++){ //Get 150 samples
    AcsValue = analogRead(A0);     //Read current sensor values   
    Samples = Samples + AcsValue;  //Add samples together
    delay (3); // let ADC settle before next sample 3ms
  }
  AvgAcs=Samples/150.0;//Taking Average of Samples
  
  //((AvgAcs * (5.0 / 1024.0)) is converitng the read voltage in 0-5 volts
  //2.5 is offset(I assumed that arduino is working on 5v so the viout at no current comes
  //out to be 2.5 which is out offset.
  //0.100v(100mV) is rise in output voltage when 1A current flows at input
  currentValue = abs((2.5 - (AvgAcs * (5.0 / 1024.0)) )/0.100);
  //Serial.print("Voltage: ");
  //Serial.print(voltageValue);
  //Serial.print("V ");
  //Serial.print(" Current: ");
  //Serial.print(currentValue);
  //Serial.print("A");
  float power = voltageValue*currentValue;
  msec = millis();
  elasped_msec = msec - last_msec; //Calculate how long has past since last call of this function
  elasped_time = elasped_msec / 1000.0; // 1sec=1000 msec
  ampSecs = (currentValue*elasped_time); //AmpSecs since last measurement
  wattSecs = ampSecs * voltageValue; //WattSecs since last measurement
  ampHours = ampHours + ampSecs/3600; // 1 hour=3600sec //Total ampHours since program started
  wattHours = wattHours + wattSecs/3600; // 1 hour=3600sec //Total wattHours since program started
  last_msec = msec; //Store 'now' for next time  
  //Serial.print("Light Sensor=");
  //Serial.print(LightSensorValue);
  //Serial.print("Rain Sensor=");
  //Serial.print(RainSensorValue);
  Serial.println();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("I=");
  lcd.print(currentValue);  
  lcd.setCursor(9, 0);
  lcd.print(wattHours);
  lcd.print("Wh");
  lcd.setCursor(0, 1);
  lcd.print("V=");
  lcd.print(voltageValue);  
  lcd.setCursor(9, 1);
  lcd.print(power);
  lcd.print("W");
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["voltage"] = voltageValue;
  root["current"] = currentValue;
  if(nodeMcu.available()>0)
  {
   root.printTo(nodeMcu);
   Serial.print("Json Sent");
  }
  else{
   Serial.print("NodeMcu Not Found");    
  }
  unsigned long currentMillis = millis();
  if(digitalRead(sensor1) == LOW){  
      previousMillis1 = currentMillis;
      digitalWrite(post1,LOW);
      flag1=1;
  }
  else{    
    if (currentMillis - previousMillis1 >= interval) {
      previousMillis1 = currentMillis;
      digitalWrite(post1,HIGH);
    }
  }
  if(digitalRead(sensor2) == LOW){  
      previousMillis2 = currentMillis;
      digitalWrite(post1,LOW);
      digitalWrite(post2,LOW);
      flag2=1;
  }
  else{ 
    flag2=0;   
    if (flag1==0 && (currentMillis - previousMillis2 >= interval)) {
      previousMillis2 = currentMillis;
      digitalWrite(post1,HIGH);
      digitalWrite(post2,HIGH);
    }
  }
  if(digitalRead(sensor3) == LOW){  
      previousMillis3 = currentMillis;
      digitalWrite(post2,LOW);
      digitalWrite(post3,LOW);
      flag3=1;
  }
  else{ 
    flag3=0;   
    if (flag2==0 && (currentMillis - previousMillis3 >= interval)) {
      previousMillis3 = currentMillis;
      digitalWrite(post2,HIGH);
      digitalWrite(post3,HIGH);
    }
  }
  if(digitalRead(sensor4) == LOW){  
      previousMillis4 = currentMillis;
      digitalWrite(post3,LOW);
      digitalWrite(post4,LOW);
      flag4=1;
  }
  else{   
    flag4=0; 
    if (flag3==0 && (currentMillis - previousMillis4 >= interval)) {
      previousMillis4 = currentMillis;
      digitalWrite(post3,HIGH);
      digitalWrite(post4,HIGH);
    }
  }
  if(digitalRead(sensor5) == LOW){  
      previousMillis5 = currentMillis;
      digitalWrite(post4,LOW);
      digitalWrite(post5,LOW);
  }
  else{    
    if (flag4==0 && (currentMillis - previousMillis5 >= interval)) {
      previousMillis5 = currentMillis;
      digitalWrite(post4,HIGH);
      digitalWrite(post5,HIGH);
    }
  }
  if(digitalRead(sensor6) == LOW){  
      previousMillis6 = currentMillis;
      digitalWrite(post5,LOW);
  }
  else{    
    if (currentMillis - previousMillis6 >= interval) {
      previousMillis6 = currentMillis;
      digitalWrite(post5,HIGH);
    }
  }
}

void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    Serial1.write(Serial.read());
  }
  while(Serial1.available()) 
  {
    Serial.write(Serial1.read());
  }
}
void send_sms()
{  
    Serial1.println("AT");
    updateSerial();  
    Serial1.println("AT+CMGF=1");
    updateSerial();
    Serial1.println("AT+CMGS=\"+8801732593040\"");
    updateSerial();
    Serial1.print("Fault detected...! Post Number Two Have A Problem.");
    updateSerial();
    Serial1.write(26);
    
    Serial1.println("AT");
    updateSerial();  
    Serial1.println("AT+CMGF=1");
    updateSerial();
    Serial1.println("AT+CMGS=\"+8801767083845\"");
    updateSerial();
    Serial1.print("Fault detected...! Post Number Two Have A Problem.");
    updateSerial();
    Serial1.write(26);
}
void send_sms1()
{  
    Serial1.println("AT");
    updateSerial();  
    Serial1.println("AT+CMGF=1");
    updateSerial();
    Serial1.println("AT+CMGS=\"+8801732593040\"");
    updateSerial();
    Serial1.print("Rain detected...!");
    updateSerial();
    Serial1.write(26);
    Serial1.println("AT");
    updateSerial();  
    Serial1.println("AT+CMGF=1");
    updateSerial();
    Serial1.println("AT+CMGS=\"+8801767083845\"");
    updateSerial();
    Serial1.print("Rain detected...!");
    updateSerial();
    Serial1.write(26);
}
