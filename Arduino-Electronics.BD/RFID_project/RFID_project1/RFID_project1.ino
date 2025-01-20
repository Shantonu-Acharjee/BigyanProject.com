/**
Reader 0: Card UID:03a8491b
Reader 0: Card UID:f20ef91b
Reader 0: Card UID:870fd426
Reader 0: Card UID:5be5760d
*/
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define RST_PIN         9          
#define SS_1_PIN        7         
#define SS_2_PIN        8      
#define LED_RED         A1     
#define LED_GREEN       A2  

#define NR_OF_READERS   2

Servo door_1;

byte ssPins[] = {SS_1_PIN, SS_2_PIN};

MFRC522 mfrc522[NR_OF_READERS];   // Create MFRC522 instance.
String allowedTags[]={{"5be5760d"},{"870fd426"},{"f20ef91b"}};
int pos = 0; 
/**
 * Initialize.
 */
void setup() {

  Serial.begin(9600); 
  while (!Serial);    

  SPI.begin();        
  delay(500);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, LOW);
  door_1.attach(3);
  door_1.write(0);
  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
    mfrc522[reader].PCD_Init(ssPins[reader], RST_PIN); // Init each MFRC522 card
    Serial.print(F("Reader "));
    Serial.print(reader);
    Serial.print(F(": "));
    mfrc522[reader].PCD_DumpVersionToSerial();
  }
}


void loop() {
  int flag = 0;
  String strID = "" ;
  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
    // Look for new cards

    if (mfrc522[reader].PICC_IsNewCardPresent() && mfrc522[reader].PICC_ReadCardSerial()) {
      strID = dump_byte_array(mfrc522[reader].uid.uidByte, mfrc522[reader].uid.size);
      Serial.println();
      Serial.println();
      Serial.print(F("Reader "));
      Serial.print(reader);
      Serial.println(strID);

      for (int i = 0; i < sizeof(allowedTags); i++) {
        if (allowedTags[i] == strID) {
          flag = 1;
          Serial.println();
          Serial.print(F(" Card "));
          Serial.print(i);
          if(reader == 0){
            digitalWrite(LED_GREEN, HIGH);         
            Serial.print(F(" IN"));
            for(pos = 0; pos < 90; pos += 1)  
            {                                 
              door_1.write(pos);              
              delay(15);                       
            }
            digitalWrite(LED_GREEN, LOW);                   
            for(pos = 90; pos>=1; pos-=1)    
            {              
              door_1.write(pos);             
              delay(15);                      
            }
          }
          else{
            digitalWrite(LED_GREEN, HIGH); 
            Serial.print(F(" OUT"));
            for(pos = 0; pos < 90; pos += 1)  
            {                                 
              door_1.write(pos);             
              delay(15);                       
            }
            for(pos = 90; pos>=1; pos-=1)    
            {              
              door_1.write(pos);              
              delay(15);                      
            }  
            digitalWrite(LED_GREEN, LOW); 
            digitalWrite(LED_RED, LOW);             
          }
        }
      }
      if(flag == 0){       
        digitalWrite(LED_RED, HIGH);  
        delay(500); 
        digitalWrite(LED_RED, LOW);
      }
      

      // Halt PICC
      mfrc522[reader].PICC_HaltA();
      mfrc522[reader].PCD_StopCrypto1();
    } 
  } 
}

/**
 * Helper routine to dump a byte array as hex values to Serial.
 */
String dump_byte_array(byte *buffer, byte bufferSize) {
  String temp = "" ;
  for (byte i = 0; i < bufferSize; i++) {
    temp += (buffer[i] < 0x10 ? "0" : "")+String(buffer[i], HEX);
  }
  return temp;
}

