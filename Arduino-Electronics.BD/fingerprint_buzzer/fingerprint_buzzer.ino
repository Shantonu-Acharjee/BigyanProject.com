#include "Adafruit_Fingerprint.h" //fingerprint library header file
#include <SoftwareSerial.h>
SoftwareSerial fingerPrint(2, 3); //for tx/rx communication between arduino & r305 fingerprint sensor



uint8_t id;
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&fingerPrint);

#define register_back 14
#define delete_ok 15
#define forward 16
#define reverse 17
#define match 5
#define indFinger 13
#define buzzer 7

#define records 10 // 10 for 10 user


void setup()
{
  delay(1000);
  Serial.begin(9600);
  pinMode(register_back, INPUT_PULLUP);
  pinMode(delete_ok, INPUT_PULLUP);
  pinMode(match, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  pinMode(indFinger, OUTPUT);
  digitalWrite(buzzer, HIGH);

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

  
  digitalWrite(indFinger, LOW);

}

void loop()
{
  
  int result = getFingerprintIDez();
  if (result > 0)
  {
    return;
  }
  checkKeys();
  delay(300);
}


void checkKeys()
{
  if (digitalRead(register_back) == 0)
  {
    digitalWrite(indFinger, HIGH);
    Serial.print("Please Wait");
    Enroll();
  }
  else{
    digitalWrite(indFinger, LOW);
    Serial.println("No Finger");
  }
}

void Enroll()
{
  id = 1;
  Serial.print("Enter Finger ID:");
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
  
    digitalWrite(buzzer, LOW);
    delay(5000);
    digitalWrite(buzzer, HIGH);
  return finger.fingerID;
}
