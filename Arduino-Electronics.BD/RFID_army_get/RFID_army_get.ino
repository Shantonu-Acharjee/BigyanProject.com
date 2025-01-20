#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

Servo myservo;
MFRC522 rfid(10, 8);
int door_in[1][4]={
  {0, 0, 0, 0}   
};
int fget_status = 0;
int sget_status = 0;
int pos = 90;
String uidString;
void setup() {
  Serial.begin(9600);             
  pinMode(6,INPUT);           
  pinMode(7,INPUT);
  SPI.begin(); 
  rfid.PCD_Init(); 
  
  
  
  Serial.println("initialization done.");
  myservo.attach(9);
  myservo.write(pos);
  delay(3000);
  
}

void loop() {  
  uidString = " ";
  if(digitalRead(6) == LOW){
    Serial.println("entry");
    fget_status = 1;
    for (pos = 90; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15 ms for the servo to reach the position
    }
  }
  if(digitalRead(7) == LOW && fget_status == 2){
    Serial.println("exit");
    for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15 ms for the servo to reach the position
    }
    
  }
  if(fget_status == 1){
    if(rfid.PICC_IsNewCardPresent()) {
      readRFID();
    }
  }
  delay(10);
}
void readRFID() {  
  rfid.PICC_ReadCardSerial();
  Serial.print("Tag UID: ");
  uidString = String(rfid.uid.uidByte[0]) + String(rfid.uid.uidByte[1]) + String(rfid.uid.uidByte[2]) + String(rfid.uid.uidByte[3]);
  Serial.println(uidString);
  if(uidString == "1431569031")
  {
    fget_status = 2;
  }
  if(uidString == "4233130163")
  {
    fget_status = 2;
  }
}
