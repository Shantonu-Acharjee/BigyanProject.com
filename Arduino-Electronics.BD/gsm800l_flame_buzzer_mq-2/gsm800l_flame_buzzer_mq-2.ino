#include <SoftwareSerial.h>
int buzzer = 9;
int flame_sensor = 7;
int smoke_sensor = 8;
SoftwareSerial mySerial(3, 2);
int flag = 0;
void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
  Serial.println("Initializing..."); 
  delay(1000);
  pinMode(buzzer, OUTPUT);
  pinMode(flame_sensor, INPUT);
  pinMode(smoke_sensor, INPUT);
  digitalWrite(buzzer, LOW);
}

void loop()
{
  if (!digitalRead(flame_sensor) || !digitalRead(smoke_sensor))
  {
    Serial.println("Fire detected...! take action immediately.");
    digitalWrite(buzzer, HIGH);
    send_sms();
    delay(200);
  }
  else
  {
    Serial.println("No flame detected. stay cool");
    digitalWrite(buzzer, LOW);
  }
  delay(1000);
}

void send_sms()
{  
  if(flag == 0){
    mySerial.println("AT");
    updateSerial();
  
    mySerial.println("AT+CMGF=1");
    updateSerial();
    mySerial.println("AT+CMGS=\"+8801742016853\"");
    updateSerial();
    mySerial.print("Fire detected...! take action immediately.");
    updateSerial();
    mySerial.write(26);
    flag = 1;
  }
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
