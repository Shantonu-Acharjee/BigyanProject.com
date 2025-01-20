#include <SoftwareSerial.h>
#include <SPI.h>
#include <Wire.h> 
#include <SD.h>
#include <DS3231.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
SoftwareSerial mySerial(3, 2);
DS3231  rtc(SDA, SCL);
MFRC522 rfid(48, 49);
int door_in[1][4]={
  {0, 0, 0, 0}   
};
int rollnumber = 0;
String uidString;
void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);               
  lcd.init();
  lcd.backlight();
  rtc.begin();
  SPI.begin(); 
  rfid.PCD_Init(); 
  Serial.print("Initializing SD card...");
  lcd.print("Initializing ");
  lcd.setCursor(0, 1);
  lcd.print("SD card...");
  delay(3000);
  lcd.clear();
  if(!SD.begin(47)) {
    Serial.println("initialization failed!");
    lcd.print("Initializing ");
    lcd.setCursor(0, 1);
    lcd.print("failed!");
    return;
  }
  Serial.println("initialization done.");
  lcd.print("Initialization ");
  lcd.setCursor(0, 1);
  lcd.print("Done...");
  delay(3000);
  lcd.clear();
  lcd.print("Put RFID to Scan");
}

void loop() {  
    rollnumber = 0;
    uidString = " ";
  if(rfid.PICC_IsNewCardPresent()) {
    readRFID();
    logCard();
    verifyCheckIn();
  }
  delay(10);
}
void readRFID() {  
  rfid.PICC_ReadCardSerial();
  Serial.print("Tag UID: ");
  uidString = String(rfid.uid.uidByte[0]) + String(rfid.uid.uidByte[1]) + String(rfid.uid.uidByte[2]) + String(rfid.uid.uidByte[3]);
  Serial.println(uidString);
  if(uidString == "1836811164")
  {
    rollnumber = 1;
  }
  if(uidString == "11987959")
  {
    rollnumber = 2;
  }
  if(uidString == "234316795")
  {
    rollnumber = 3;
  }
  if(uidString == "8722111496")
  {
    rollnumber = 4;
  }
}

void logCard() {  
  File myFile;
  digitalWrite(47,LOW);
  myFile=SD.open("DATA.txt", FILE_WRITE);
  if (myFile) {
    Serial.println("File opened ok");   
    lcd.clear();
    lcd.setCursor(0, 0);
    if((int)door_in[0][rollnumber] != rollnumber){      
      lcd.print("Welcome Roll: ");
      lcd.print(rollnumber);
      myFile.print("Roll# ");
      myFile.print(rollnumber);
      myFile.print(" Entered at "); 
      String msg = "Roll# " + String(rollnumber) + " has Reached Class";     
      mySerial.println("AT");
      updateSerial();
    
      mySerial.println("AT+CMGF=1");
      updateSerial();
      if(rollnumber == 1){
        mySerial.println("AT+CMGS=\"+8801912987447\"");
      }
      if(rollnumber == 2){
        mySerial.println("AT+CMGS=\"+8801998887605\"");
      }
      if(rollnumber == 3){
        mySerial.println("AT+CMGS=\"+8801738294322\"");
      }
      if(rollnumber == 4){
        mySerial.println("AT+CMGS=\"+8801838595640\"");
      }      
      updateSerial();
      mySerial.print(msg);
      updateSerial();
      mySerial.write(26);
      delay(200);
      door_in[0][rollnumber]=rollnumber;
    }
    else{    
      lcd.print("Thank you Roll:");
      lcd.print(rollnumber);
      myFile.print("Roll# ");
      myFile.print(rollnumber);
      myFile.print(" Left Class at ");
      String msg = "Roll# " + String(rollnumber) + " has left Class";     
      mySerial.println("AT");
      updateSerial();    
      mySerial.println("AT+CMGF=1");
      updateSerial();
      
      if(rollnumber == 1){
        mySerial.println("AT+CMGS=\"+8801912987447\"");
      }
      if(rollnumber == 2){
        mySerial.println("AT+CMGS=\"+8801998887605\"");
      }
      if(rollnumber == 3){
        mySerial.println("AT+CMGS=\"+8801738294322\"");
      }
      if(rollnumber == 4){
        mySerial.println("AT+CMGS=\"+8801838595640\"");
      }   
      updateSerial();
      mySerial.print(msg);
      updateSerial();
      mySerial.write(26);
      delay(200);
      door_in[0][rollnumber]=0;
    }  
    
    Time  t;
    t = rtc.getTime();
    myFile.print(t.date);
    myFile.print('/');
    myFile.print(t.mon);
    myFile.print('/');
    myFile.print(t.year);
    myFile.print(',');
    myFile.print(t.hour);
    myFile.print(':');
    myFile.println(t.min);
    Serial.print(t.year);
    Serial.print('/');
    Serial.print(t.mon);
    Serial.print('/');
    Serial.print(t.date);
    Serial.print(' ');
    Serial.print(t.hour);
    Serial.print(':');
    Serial.println(t.min);
    Serial.println("sucessfully written on SD card");
    delay(2000);    
    myFile.close();    
    delay(200);
  }
  else {    
    Serial.println("error opening data.txt");  
    lcd.clear();
    lcd.print("error opening data.txt");
  } 
  digitalWrite(47,HIGH);
}

void verifyCheckIn(){
    delay(1000);
    lcd.clear();
    lcd.print("Put RFID to Scan");
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
