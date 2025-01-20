#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
const int rs = 9, en = 8, d4 = 7, d5 = 6, d6 = 5, d7 = 4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int buzzer = 11;
int smoke_sensor = 12;
SoftwareSerial mySerial(3, 2);
int flag = 0;
void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
  Serial.println("Initializing..."); 
  delay(1000);
  pinMode(buzzer, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(smoke_sensor, INPUT);
  digitalWrite(buzzer, LOW);
  digitalWrite(10, HIGH);
  lcd.begin(16, 2);
  lcd.print("Monitoring......");
}

void loop()
{
  if (!digitalRead(smoke_sensor))
  {
    Serial.println("Gas detected...! take action immediately.");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Alert!!!");
    lcd.setCursor(0, 1);
    lcd.print("Gas detected...!");
    digitalWrite(buzzer, HIGH);
    send_sms();
    delay(200);
  }
  else
  {
    Serial.println("No Gas detected. stay cool");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Stay Cool");
    lcd.setCursor(0, 1);
    lcd.print("No Gas detected");
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
    mySerial.println("AT+CMGS=\"+8801719903669\"");
    updateSerial();
    mySerial.print("Gas detected...! take action immediately.");
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
