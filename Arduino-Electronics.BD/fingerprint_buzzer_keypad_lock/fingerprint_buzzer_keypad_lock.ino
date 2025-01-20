#include <Keypad.h>
#include "Adafruit_Fingerprint.h" //fingerprint library header file
#include <SoftwareSerial.h>
#include<EEPROM.h>
SoftwareSerial fingerPrint(18, 19); //for tx/rx communication between arduino & r305 fingerprint sensor

uint8_t id;
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&fingerPrint);

#define relay_pin 10
#define buzzer 11
#define RedLed 13
#define GreenLed 12

char password[4];
char initial_password[4],new_password[4];
int i=0;
int flag = 0;
char key_pressed=0;
const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );




void setup(){

  Serial.begin(9600);
  pinMode(buzzer, OUTPUT);
  pinMode(RedLed, OUTPUT);
  pinMode(GreenLed, OUTPUT);
  pinMode(relay_pin, OUTPUT);
  digitalWrite(buzzer, HIGH);
  digitalWrite(relay_pin, HIGH);
  finger.begin(57600);
  Serial.begin(9600);
  delay(2000);
  if (finger.verifyPassword())
  {
    Serial.println("Found fingerprint sensor!");
    delay(2000);
  }
  else
  {
    Serial.println("Did not find fingerprint sensor :(");
    while (1);
  }

   initialpassword();
  //digitalWrite(indFinger, LOW);
  
}
  
void loop(){
  key_pressed = keypad.getKey();
  if(key_pressed)
  {    
    digitalWrite(buzzer, LOW);
    delay(100);
    digitalWrite(buzzer, HIGH);
    Serial.print(key_pressed);
    if(key_pressed == '*'){
      flag = 0;
      i=0;
      digitalWrite(RedLed, HIGH);
      change();
      digitalWrite(RedLed, LOW);
    }
    else if(key_pressed == '#'){ 
      flag = 0;
      i=0;
      digitalWrite(RedLed, HIGH);
      Enroll();
      digitalWrite(RedLed, LOW);
    }
    else{
      flag = 0;
      password[i++]=key_pressed;
    }
  }
  if(i==4)
  {
    delay(200);
    for(int j=0;j<4;j++)
      initial_password[j]=EEPROM.read(j);
    if(!(strncmp(password, initial_password,4)))
    {
      Serial.println("Pass Accepted");
      flag = 1;
      Serial.println("Enter Password:");
      i=0;
    }
    else
    {
      digitalWrite(relay_pin, HIGH);
      Serial.println("Wrong Password");
      Serial.println("Pres # to Change");
      delay(2000);
      Serial.println("Enter Password");
      i=0;
    }
  }
  int result = getFingerprintIDez();
  if (result > 0)
  {
    return;
  }
}

void Enroll()
{
  id = 1;
  Serial.println("Enter Finger ID:");
  getFingerprintEnroll();  
}


uint8_t getFingerprintEnroll()
{
  int p = -1;
  Serial.print("finger ID:");
  Serial.print(id);
  Serial.print("Place Finger");
  digitalWrite(buzzer, LOW);
  delay(1000);
  digitalWrite(buzzer, HIGH);
  delay(2000);
  while (p != FINGERPRINT_OK)
  {
    p = finger.getImage();
    switch (p)
    {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        erralarm();
        Serial.println("No Finger");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        erralarm();
        Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        erralarm();
        Serial.println("Imaging error");
        break;
      default:
        erralarm();
        Serial.println("Unknown error");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      erralarm();
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      erralarm();
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      erralarm();
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      erralarm();
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      erralarm();
      Serial.println("Unknown error");
      return p;
  }

  Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  
  digitalWrite(buzzer, LOW);
  delay(1000);
  digitalWrite(buzzer, HIGH);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.print(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        erralarm();
        Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        erralarm();
        Serial.println("Imaging error");
        break;
      default:
        erralarm();
        Serial.println("Unknown error");
        return;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      erralarm();
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      erralarm();
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      erralarm();
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      erralarm();
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      erralarm();
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      erralarm();
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  Serial.print("Creating model for #"); Serial.println(id);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    erralarm();
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {   
    erralarm();
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    erralarm();
    Serial.println("Unknown error");
    return p;
  }

  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");    
    digitalWrite(buzzer, LOW);
    delay(1000);
    digitalWrite(buzzer, HIGH);
    delay(2000);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    erralarm();
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    erralarm();
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    erralarm();
    Serial.println("Error writing to flash");
    return p;
  }
  else {
    Serial.println("Unknown error");
    erralarm();
    return p;
  }
}
void erralarm(){  
    digitalWrite(buzzer, LOW);
    delay(100);
    digitalWrite(buzzer, HIGH);
    delay(100);  
    digitalWrite(buzzer, LOW);
    delay(100);
    digitalWrite(buzzer, HIGH);
}
int getFingerprintIDez()
{
  uint8_t p = finger.getImage();

  if (p != FINGERPRINT_OK)
    return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)
    return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)
  {
    Serial.print("Finger Not Found");
    Serial.print("Try Later");
    delay(2000);
    return -1;
  }
  // found a match!
  Serial.print("Found ID #");
  Serial.print(finger.fingerID);
  if(flag == 1){    
    digitalWrite(GreenLed, HIGH);
    digitalWrite(relay_pin, LOW);
    digitalWrite(buzzer, LOW);
    delay(5000);
    digitalWrite(buzzer, HIGH);
    digitalWrite(relay_pin, HIGH);
    digitalWrite(GreenLed, LOW);
    flag = 0;
  }
  return finger.fingerID;
}
void change()
{
  int j=0;
  Serial.println("Current Password");
  while(j<4)
  {
    char key=keypad.getKey();
    if(key)
    {      
      digitalWrite(buzzer, LOW);
      delay(100);
      digitalWrite(buzzer, HIGH);
      new_password[j++]=key;
      Serial.print(key);
    }
    key=0;
  }
  delay(500);
  if((strncmp(new_password, initial_password, 4)))
  {
    Serial.println("Wrong Password");
    Serial.println("Try Again");
    delay(1000);
  }
  else
  {
    j=0;
    Serial.println("New Password:");
    while(j<4)
    {
      char key=keypad.getKey();
      if(key)
      {
        digitalWrite(buzzer, LOW);
        delay(100);
        digitalWrite(buzzer, HIGH);
        initial_password[j]=key;
        Serial.print(key);
        EEPROM.write(j,key);
        j++;
      }
    }
    Serial.println("Pass Changed");
    delay(1000);
  }
  Serial.println("Enter Password");
  key_pressed=0;
}

void initialpassword(){
  for(int j=0;j<4;j++)
    EEPROM.write(j, j+49);
  for(int j=0;j<4;j++)
    initial_password[j]=EEPROM.read(j);
}
