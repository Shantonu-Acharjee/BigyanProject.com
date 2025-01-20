#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <Servo.h>

#define Servo_PIN 9
#define FSwitch_PIN 12
#define Relay1_PIN 6
#define Relay2_PIN 5
#define TiltSwitch_PIN 4
#define GSMRX_PIN 3
#define GSMTX_PIN 2
#define GAS_PIN 7

int flag = 0;
String message = "";   // A String for storing the message

Servo myservo;

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

SoftwareSerial SIM800(GSMTX_PIN, GSMRX_PIN);

void setup()
{
  Serial.begin(9600);
  pinMode(FSwitch_PIN,INPUT_PULLUP);
  pinMode(Relay1_PIN,OUTPUT);
  pinMode(Relay2_PIN,OUTPUT);
  pinMode(TiltSwitch_PIN,INPUT);
  pinMode(GAS_PIN,INPUT);
  myservo.attach(Servo_PIN);
  digitalWrite(Relay1_PIN, LOW);
  digitalWrite(Relay2_PIN, HIGH);
  lcd.init();                      // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(3,0);
  lcd.print(" Welcome To");
  lcd.setCursor(2,1);
  lcd.print(" Smart Manhole");
  lcd.setCursor(0,2);
  lcd.print("      System");
  lcd.setCursor(2,3);
  lcd.print("");
  myservo.write(0);
  SIM800.begin(9600); // baudrate for GSM shield
  // set SMS mode to text mode
  SIM800.print("AT+CMGF=1\r");  
  delay(100);  
  // set gsm module to tp show the output on serial out
  SIM800.print("AT+CNMI=2,2,0,0,0\r");
}


void loop()
{ 
  Serial.print(digitalRead(TiltSwitch_PIN));
  Serial.print("=");
  Serial.print(digitalRead(FSwitch_PIN));
  delay(500);
  Serial.println();
  lcd.clear();
  if(!digitalRead(TiltSwitch_PIN)){
    lcd.setCursor(0,0);
    lcd.print(" Manhole Opened");
  }
  if(!digitalRead(FSwitch_PIN)){
    digitalWrite(Relay2_PIN, LOW);
    myservo.write(90);
    message = "Manhole Full Turning on resurved Pipe line.";
    send_message(message);
    
    lcd.setCursor(0,1);
    lcd.print(" Manhole Full");
  }
  else{
    myservo.write(0);
    digitalWrite(Relay2_PIN, HIGH);
  }
  if(!digitalRead(GAS_PIN)){    
    message = "Danger Gas Detected in Pipe line.";
    send_message(message);
    Serial.println(message);    
    lcd.setCursor(0,2);
    lcd.print("Danger Gas Detected");
  }
}
void send_message(String message)
{
  SIM800.listen();
  SIM800.println("AT+CMGF=1");    //Set the GSM Module in Text Mode
  delay(100);  
  SIM800.println("AT+CMGS=\"+8801791774606\""); // Replace it with your mobile number
  delay(100);
  SIM800.println(message);   // The SMS text you want to send
  delay(100);
  SIM800.println((char)26);  // ASCII code of CTRL+Z
  delay(100);
  SIM800.println();
  delay(1000);  
}
