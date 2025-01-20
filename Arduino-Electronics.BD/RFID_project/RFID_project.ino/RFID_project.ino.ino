/**
Reader 0: Card UID:79661863
Reader 0: Card UID:7b14e50d
Reader 0: Card UID:69003663
Reader 0: Card UID:a90e2763
Reader 0: Card UID:0357891b
Reader 0: Card UID:0328b61b
Reader 0: Card UID:b3e25c1a
Reader 0: Card UID:03231c1b
*/
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define RST_PIN         9          
#define SS_1_PIN        2         
#define SS_2_PIN        8         
#define SS_3_PIN        7         
#define SS_4_PIN        4     
#define LED_RED         A2     
#define LED_GREEN       A1     
#define BUZZER          A0 

#define NR_OF_READERS   4

Servo door_1;
Servo door_2;
Servo door_3;
Servo door_4;

byte ssPins[] = {SS_1_PIN, SS_2_PIN, SS_3_PIN, SS_4_PIN};

MFRC522 mfrc522[NR_OF_READERS];   // Create MFRC522 instance.
int tempo = 100;
char names[] = {'D', 'E', 'F', 'G', 'A', 'B', 'C', 'd'}; //names of notes
int frequencies[] = {587, 699, 880, 1047, 1319, 1568, 1976, 2093}; //note frequencies
char notes[] = "cffefaagCCCvagacffefaagCffeedcCCCfvaagCCCfvaagDDDCvavgavCffgaDDDCvavgavCffgf ";
String allowedTags[]={{"79661863"},{"7b14e50d"},{"69003663"},{"a90e2763"},{"0357891b"},{"0328b61b"},{"b3e25c1a"},{"03231c1b"}};
int Points[]={100,100,100,100,100,100,100,100};
String door_in[NR_OF_READERS][8]={
  {"", "", "", "", "", "", "", ""}, 
  {"", "", "", "", "", "", "", ""}, 
  {"", "", "", "", "", "", "", ""}, 
  {"", "", "", "", "", "", "", ""}   
};
int pos = 0; 
int Limit = 30;
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
  pinMode(BUZZER, OUTPUT);
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(BUZZER, LOW);
  door_1.attach(3);
  door_2.attach(5);
  door_3.attach(6);
  door_4.attach(10);
  door_1.write(0);
  door_2.write(0);
  door_3.write(0);
  door_4.write(0);
  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
    mfrc522[reader].PCD_Init(ssPins[reader], RST_PIN); // Init each MFRC522 card
    Serial.print(F("Reader "));
    Serial.print(reader);
    Serial.print(F(": "));
    mfrc522[reader].PCD_DumpVersionToSerial();
  }
}


void loop() {
  String strID = "" ;
  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
    // Look for new cards

    if (mfrc522[reader].PICC_IsNewCardPresent() && mfrc522[reader].PICC_ReadCardSerial()) {
      strID = dump_byte_array(mfrc522[reader].uid.uidByte, mfrc522[reader].uid.size);
      Serial.println();
      Serial.println();
      Serial.print(F("Reader "));
      Serial.print(reader);
      //Serial.println(strID);

      for (int i = 0; i < sizeof(allowedTags); i++) {
        if (allowedTags[i] == strID) {
          Serial.println();
          Serial.print(F(" Card "));
          Serial.print(i);
          if(reader == 0 || reader == 2){
            if(door_in[0][i] != strID && door_in[2][i] != strID){
              if(Points[i] >= Limit){ 
                digitalWrite(LED_GREEN, HIGH); 
                tone(BUZZER, frequency[i], 1000);         
                Serial.print(F(" IN"));
                Serial.println();
                Serial.print(F("Card Balance : "));
                Serial.print(Points[i]);
                door_in[reader][i]=strID; 
                if(reader == 0)
                {
                  for(pos = 0; pos < 90; pos += 1)  
                  {                                 
                    door_1.write(pos);              
                    door_2.write(pos);             
                    delay(15);                       
                  }
                  digitalWrite(LED_GREEN, LOW); 
                  noTone(BUZZER);   
                  digitalWrite(LED_RED, HIGH); 
                  for(pos = 90; pos>=1; pos-=1)    
                  {              
                    door_1.write(pos);              
                    door_2.write(pos);             
                    delay(15);                      
                  }
                  digitalWrite(LED_RED, LOW); 
                  digitalWrite(LED_GREEN, LOW); 
                }            
                else
                {
                  digitalWrite(BUZZER, HIGH);   
                  digitalWrite(LED_GREEN, HIGH); 
                  for(pos = 0; pos < 90; pos += 1)  
                  {                                 
                    door_3.write(pos);              
                    door_4.write(pos);             
                    delay(15);                       
                  }
                  digitalWrite(LED_GREEN, LOW); 
                  digitalWrite(BUZZER, LOW); 
                  digitalWrite(LED_RED, HIGH); 
                  for(pos = 90; pos>=1; pos-=1)    
                  {              
                    door_3.write(pos);              
                    door_4.write(pos);             
                    delay(15);                      
                  }
                  digitalWrite(LED_RED, LOW); 
                  digitalWrite(LED_GREEN, LOW);
                }
              }
              else
              {
                Serial.println();
                Serial.print(F("Not Enough Balance"));
              }
            }
            else{
              Serial.println();
              Serial.print(F("This Card Already Present!"));
            }
          }
          else{
            if(door_in[0][i] == strID || door_in[2][i] == strID){
              if(reader == 3){
                digitalWrite(LED_GREEN, HIGH);
                tone(BUZZER, frequency[i], 1000);  
                digitalWrite(LED_RED, LOW); 
                Serial.print(F(" OUT"));
                if(door_in[0][i] == strID){
                  Points[i] = Points[i] - 10;
                  Serial.println();
                  Serial.print(F("Card Balance Now: "));
                  Serial.print(Points[i]);
                }
                door_in[0][i] = " ";
                door_in[2][i] = " ";
                for(pos = 0; pos < 90; pos += 1)  
                {                                 
                  door_3.write(pos);              
                  door_4.write(pos);             
                  delay(15);                       
                }
                digitalWrite(LED_GREEN, LOW);
                noTone(BUZZER);    
                digitalWrite(LED_RED, HIGH); 
                for(pos = 90; pos>=1; pos-=1)    
                {              
                  door_3.write(pos);              
                  door_4.write(pos);             
                  delay(15);                      
                }  
                digitalWrite(LED_GREEN, LOW); 
                digitalWrite(LED_RED, LOW);             
              }
              else if(reader == 1){
                digitalWrite(LED_GREEN, HIGH); 
                digitalWrite(LED_RED, LOW); 
                Serial.print(F(" OUT"));
                if(door_in[2][i] == strID){
                  Points[i] = Points[i] - 10;
                  Serial.println();
                  Serial.print(F("Card Balance Now: "));
                  Serial.print(Points[i]);
                  
                  /*for( int k = 0; k < 2; ++k ) {
                    // loop through columns of current row
                    for( int j = 0; j < 8; ++j ){
                      Serial.print(door_in[ k ][ j ] );
                      Serial.println() ;
                      Serial.print("K=");
                      Serial.println(k); // start new line of output
                      Serial.print("J=");
                      Serial.println(j);
                    }
                 }*/
                }                
                door_in[0][i] = " ";
                door_in[2][i] = " ";  
                for(pos = 0; pos < 90; pos += 1)  
                {                                 
                  door_1.write(pos);              
                  door_2.write(pos);             
                  delay(15);                       
                }
                digitalWrite(LED_GREEN, HIGH); 
                digitalWrite(LED_RED, LOW); 
                for(pos = 90; pos>=1; pos-=1)    
                {              
                  door_1.write(pos);              
                  door_2.write(pos);             
                  delay(15);                      
                } 
                digitalWrite(LED_GREEN, LOW); 
                digitalWrite(LED_RED, LOW); 
              }
            }
            else{
              Serial.println();
              Serial.print(F("This Card Not Present Yet!"));
            }
          }
        }
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

int frequency(char note)  
{   
  int i;          // Variable i defined   
const int numNotes = 14;           // Number of notes stored which is 14 (2 octaves)   
for (i = 0; i < numNotes; i++)    // For loop is used to search through the letters   
{     
  if (names[i] == note)                // If this is a specified note     
  {      
    return(frequencies[i]);            // Return the frequency     
  }   
}   return(0);
// Otherwise don't return anything 
}
