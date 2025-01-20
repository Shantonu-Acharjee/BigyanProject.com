#include <SoftwareSerial.h>
int buzzer = 5;
int smoke_sensor = 2;
SoftwareSerial mySerial(4, 3);
int flag = 0;
void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
  Serial.println("Initializing..."); 
  delay(1000);
  pinMode(buzzer, OUTPUT);
  pinMode(smoke_sensor, INPUT);
  digitalWrite(buzzer, HIGH);
}

void loop()
{
  if (!digitalRead(smoke_sensor))
  {
    Serial.println("Fire detected...! take action immediately.");
    digitalWrite(buzzer, LOW);    
    make_call();
    send_sms();
    delay(200);
  }
  else
  {
    Serial.println("No flame detected. stay cool");
    digitalWrite(buzzer, HIGH);
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
    mySerial.println("AT+CMGS=\"+8801814849438\"");
    updateSerial();
    mySerial.print("Fire detected...! take action immediately.");
    updateSerial();
    mySerial.write(26);
    flag = 1;
  }
}
void make_call(){
  if(flag == 0){
    mySerial.println("AT");
    updateSerial();  
    mySerial.println("ATD+ +8801814849438;");
    updateSerial();
    delay(20000); // wait for 20 seconds...
    mySerial.println("ATH"); //hang up
    updateSerial();
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
