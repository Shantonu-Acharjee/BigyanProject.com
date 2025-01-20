#include <SoftwareSerial.h>
SoftwareSerial mySerial(8, 9); // RX, TX Serial communication for SIM800L
int buzzerPin = 4;
int LaserPin = 2;
int LdrPin = 12;
void setup()
{
  Serial.begin(115200); // Begin serial Communication with Baud rate 115200 for Debuging
  mySerial.begin(115200); // Begin serial Communication with Baud rate 115200 for SIM800L
  pinMode(buzzerPin,OUTPUT);
  pinMode(LaserPin,OUTPUT);
  pinMode(LdrPin,INPUT);
  digitalWrite(buzzerPin,HIGH);
  Serial.println("Initializing..."); 
  delay(1000);
  //delay(15000);
  digitalWrite(LaserPin,HIGH);
  Serial.println("done...");
  mySerial.println("AT"); //If everything is Okay it will show "OK" on the serial monitor
  updateSerial();
  mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  mySerial.println("AT+CNMI=2,2,0,0,0"); //Configure the SIM800L on how to manage the Received SMS... Check the SIM800L AT commands manual
  updateSerial();
}

void loop()
{
  // If LDR pin Low mean something in front of laser beam.
  if(digitalRead(LdrPin) == 1){
    Serial.println("high"); // for debuging
    digitalWrite(buzzerPin,LOW); // start buzzer.
    make_call();
    send_sms();
  }
  else{
    Serial.println("low");
  }
  updateSerial();  
}

void make_call()
{  
  mySerial.println("AT");
  updateSerial();
  mySerial.println("ATD+ +8801742452449;");
  updateSerial();
}
void send_sms()
{  
  mySerial.println("AT");
  updateSerial();

  mySerial.println("AT+CMGF=1");
  updateSerial();
  mySerial.println("AT+CMGS=\"+8801742452449\"");
  updateSerial();
  mySerial.print("Security Alert! take action immediately.");
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
