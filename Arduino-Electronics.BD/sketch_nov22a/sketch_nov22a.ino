#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 9); // RX, TX
#include <Servo.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#define trigPin1 7
#define echoPin1 8
Servo myservo;
#define ONE_WIRE_BUS 11
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
int pos;
void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(4,INPUT);
  pinMode(5,INPUT);
  pinMode(6,INPUT);
  pinMode(2,OUTPUT);
  pinMode(13,OUTPUT);
  myservo.attach(3);
  myservo.write(90);
  digitalWrite(3,HIGH);
  digitalWrite(13,HIGH);
  sensors.begin();
  Serial.println("done...");
  mySerial.println("AT"); //If everything is Okay it will show "OK" on the serial monitor
  updateSerial();
  mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  mySerial.println("AT+CNMI=2,2,0,0,0"); //Configure the SIM800L on how to manage the Received SMS... Check the SIM800L AT commands manual
  updateSerial();
}

void loop() {
  sensors.requestTemperatures(); 
  int temperatureC = sensors.getTempCByIndex(0);
  Serial.print(temperatureC);
  Serial.println("ºC");
  updateSerial();
  if(temperatureC >= 30)
  {
    digitalWrite(13,LOW);
  }
  else{
    digitalWrite(13,HIGH);
  }
  if(digitalRead(5)){
    //Serial.println("high");
    digitalWrite(2,HIGH);
  }
  else{
    digitalWrite(13,LOW);
    send_sms();
    Serial.println("low");
    digitalWrite(2,LOW);
    for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
    }
    for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
    digitalWrite(13,HIGH);
  }
}
void send_sms()
{  
  mySerial.println("AT");
  updateSerial();

  mySerial.println("AT+CMGF=1");
  updateSerial();
  mySerial.println("AT+CMGS=\"+8801841253981\""); //8801841253981
  updateSerial();
  mySerial.print("Fire detected...! take action immediately.");
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
