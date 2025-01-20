#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>
#define S0 12
#define S1 13
#define S2 10
#define S3 11
#define sensorOut 9
SoftwareSerial mySerial(3, 2); // RX, TX
SoftwareSerial node(5, 4); // RX, TX

#define ONE_WIRE_BUS 6

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);
LiquidCrystal_I2C lcd(0x27,16,2);

float Celcius=0;
float calibration_value = 21.34-0.5;
int phval = 0; 
unsigned long int avgval; 
int buffer_arr[10],temp;
char incomingChar;
float ph_act;
unsigned long lastTime = millis();
unsigned long delays = 1000;
String Data_SMS;
int flag = 0;
// Stores frequency read by the photodiodes
int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;

// Stores the red. green and blue colors
int redColor = 0;
int greenColor = 0;
int blueColor = 0;
String ph = "0";
String color = "Normal";
String spe = "0";
String pressure = "0";
String flow = "0.0";
String volume = "0.0";
String str = "";
void setup() 
{ 
  Serial.begin(115200); 
  mySerial.begin(115200);
  node.begin(115200);
  Serial.println("Initializing..."); 
  delay(15000);
  Serial.println("done...");
  sensors.begin();
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  lcd.setCursor(2,0);
  lcd.print("Welcome To");
  lcd.setCursor(1,1);
  lcd.print("Water Project!");
  delay(2000);
  lcd.clear();
  ReceiveMode();
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(8, OUTPUT);
  digitalWrite(8,HIGH);
  pinMode(6, OUTPUT);
  digitalWrite(6,HIGH);
  pinMode(7, OUTPUT);
  digitalWrite(7,LOW);
  // Setting the sensorOut as an input
  pinMode(sensorOut, INPUT);
  
  // Setting frequency scaling to 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
} 

void loop(){
  unsigned long timeNow = millis();
  while(mySerial.available()>0){ 
    if(flag == 1)
    break;
    incomingChar=mySerial.read();
    Serial.print(incomingChar);
    if(incomingChar=='S') {
        flag = 1;
        //delay(10);
        Serial.print("...Request Received \n");
        Data_SMS = "Temp = "+String(Celcius,1)+" C"+" \npH ="+String(ph_act,1)+"\nColor ="+color;
        send_sms();
        ReceiveMode();
        incomingChar = 'F';
        mySerial.flush();
        delay(5000);
    }
   }
 // Setting RED (R) filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  
  // Reading the output frequency
  redFrequency = pulseIn(sensorOut, LOW);
  
  if(redFrequency < 300){
   // Printing the RED (R) value
    Serial.print("R = ");
    Serial.print(redFrequency);
    delay(100);
    
    // Setting GREEN (G) filtered photodiodes to be read
    digitalWrite(S2,HIGH);
    digitalWrite(S3,HIGH);
    // Reading the output frequency
    greenFrequency = pulseIn(sensorOut, LOW);
    
    // Printing the GREEN (G) value  
    Serial.print(" G = ");
    Serial.print(greenFrequency);
    delay(100);
   
    // Setting BLUE (B) filtered photodiodes to be read
    digitalWrite(S2,LOW);
    digitalWrite(S3,HIGH);
    
    // Reading the output frequency
    blueFrequency = pulseIn(sensorOut, LOW);
    
    // Printing the BLUE (B) value 
    Serial.print(" B = ");
    Serial.println(blueFrequency);
    delay(100);
    int lt[3] = {redFrequency, greenFrequency, blueFrequency}; 
    int minIndex = 0;
   int min = lt[minIndex];
   for (int i=1; i<3; i++){
     if (min>lt[i]){
       minIndex = i;
       min = lt[i];
     }
   }
 
    Serial.println();
    Serial.print(min);
    Serial.print( "=");
    Serial.println(minIndex);
    if(minIndex == 0){
      Serial.println("RED"); 
      color="RED";     
    }
    if(minIndex == 1){
      Serial.println("GREEN"); 
      color="GREEN";        
    }
    if(minIndex == 2){
      Serial.println("BLUE");
      color="BLUE";         
    }
 }
 else{
  color="Normal";
 }
 for(int i=0;i<10;i++) 
 { 
   buffer_arr[i]=analogRead(A0);
   delay(30);
 }
 for(int i=0;i<9;i++)
 {
   for(int j=i+1;j<10;j++)
   {
     if(buffer_arr[i]>buffer_arr[j])
     {
       temp=buffer_arr[i];
       buffer_arr[i]=buffer_arr[j];
       buffer_arr[j]=temp;
     }
   }
 }
 avgval=0;
 for(int i=2;i<8;i++)
 avgval+=buffer_arr[i];
 float volt=(float)avgval*5.0/1024/6; 
  ph_act = -5.70 * volt + calibration_value;
 sensors.requestTemperatures(); 
 Celcius=sensors.getTempCByIndex(0);
 if (timeNow - lastTime > delays)
 {
   Serial.print(" C  ");
   Serial.println(Celcius);
   
   Serial.print("pH Val: ");
   Serial.println(ph_act);
   
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("pH Val: ");
   lcd.print(ph_act);
   lcd.setCursor(0,1);
   lcd.print("Temp: ");
   lcd.print(Celcius);
   lcd.print(" C");
   str =String(ph_act)+String("=")+String(color)+String("=")+String(spe)+String("=")+String(Celcius)+String("=")+String(pressure)+String("=")+String(flow)+String("=")+String(volume);
   node.println(str);
   Serial.println("Json Sent");
   lastTime = timeNow;
  }
}
void send_sms()
{  
  mySerial.println("AT");
  updateSerial();

  mySerial.println("AT+CMGF=1");
  updateSerial();
  mySerial.println("AT+CMGS=\"+8801983715331\"");
  updateSerial();
  mySerial.print(Data_SMS);
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
void ReceiveMode(){       //Set the SIM800L Receive mode
  
  mySerial.println("AT"); //If everything is Okay it will show "OK" on the serial monitor
  updateSerial();
  mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  mySerial.println("AT+CNMI=2,2,0,0,0"); //Configure the SIM800L on how to manage the Received SMS... Check the SIM800L AT commands manual
  updateSerial();
}
