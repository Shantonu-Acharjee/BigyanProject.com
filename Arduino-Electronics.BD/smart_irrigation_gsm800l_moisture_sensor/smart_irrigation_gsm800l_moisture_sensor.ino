#include <SoftwareSerial.h> // Library for using serial communication
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

#define DHTPIN 11     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22 
SoftwareSerial SIM800(3, 2); // Pins 3, 2 are used as used as software serial pins
LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHTPIN, DHTTYPE);
String incomingData;   // for storing incoming serial data
String message = "";   // A String for storing the message
int pump = 5;    // Initialized a pin for relay module
int sensor = 4;    // Initialized a pin for relay module
int buzzer = 12;
int flag = 0;

void setup()
{
  Serial.begin(9600); // baudrate for serial monitor
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);
  pinMode(sensor, INPUT);
  pinMode(pump, OUTPUT);   // Setting erlay pin as output pin
  digitalWrite(pump, HIGH);  // Making relay pin initailly HIGH
  lcd.init(); 
  // Turn on the blacklight and print a message.
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Finding GSM");
  lcd.setCursor(0, 1);
  lcd.print("Network....");
  delay(15000);
  SIM800.begin(9600); // baudrate for GSM shield
  lcd.clear();
  lcd.print("Set SMS Mode.");
  // set SMS mode to text mode
  SIM800.print("AT+CMGF=1\r");  
  delay(100);
  
  // set gsm module to tp show the output on serial out
  SIM800.print("AT+CNMI=2,2,0,0,0\r"); 
  lcd.clear();
  lcd.print("Done.");
  delay(100);
  dht.begin();
}

void loop()
{
  delay(2000);
  float h = dht.readHumidity();
  h = h-30;
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(t);
  lcd.print(" C");
  lcd.setCursor(0, 1);
  lcd.print("Humi: ");
  lcd.print(h);
  lcd.print(" %");
  if(t > 50){   
    lcd.clear(); 
    lcd.setCursor(0, 0);
    lcd.print("Temperature");  
    lcd.setCursor(0, 1);
    lcd.print("High"); 
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SMS And Email"); 
    lcd.setCursor(0, 1);
    lcd.print("Sending..."); 
    message = "Temperature HIGH";
    send_message(message);
  }
  if(h > 50){
    Serial.print(F("  Humidity HIGH "));
    lcd.clear(); 
    lcd.setCursor(0, 0);
    lcd.print("Humidity"); 
    lcd.setCursor(0, 1);
    lcd.print("High"); 
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SMS And Email"); 
    lcd.setCursor(0, 1);
    lcd.print("Sending..."); 
    message = "Humidity HIGH";
    send_message(message);
  }
  if(digitalRead(sensor) == HIGH && flag == 0)
  { 
    Serial.println(F("  HIGH "));
    digitalWrite(buzzer, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Water Needed."); 
    lcd.setCursor(0, 1);
    lcd.print("Turning Pump ON"); 
    delay(2000);
    digitalWrite(pump, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Pump Turned ON"); 
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SMS And Email"); 
    lcd.setCursor(0, 1);
    lcd.print("Sending..."); 
    message = "Pump turned ON";
    send_message(message);
    delay(2000);
    digitalWrite(buzzer, LOW); 
    flag = 1;  
  }
  else if(digitalRead(sensor) == LOW && flag == 1)
  {
    Serial.println(F("  HIGH "));
    digitalWrite(buzzer, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("No Water Needed."); 
    lcd.setCursor(0, 1);
    lcd.print("Turning Pump OFF"); 
    delay(2000);
    digitalWrite(pump, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Pump Turned OFF"); 
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SMS And Email"); 
    lcd.setCursor(0, 1);
    lcd.print("Sending..."); 
    message = "Pump turned OFF";
    send_message(message);
    delay(2000);
    digitalWrite(buzzer, LOW); 
    flag = 0;
  }  
  //Function for receiving sms
  receive_message();

  // if received command is to turn on pump
  if(incomingData.indexOf("pump_on")>=0)
  {
    lcd.clear();
    lcd.print("SMS Received.");
    delay(1000);
    digitalWrite(pump, LOW);
    message = "Pump turned ON";
    lcd.clear();
    lcd.print(message);
    delay(1000);
    lcd.clear();
    lcd.print("Sending SMS.");
    send_message(message);
    delay(1000);
    lcd.clear();
  }
  
  // if received command is to turn off pump
  if(incomingData.indexOf("pump_off")>=0)
  {
    lcd.clear();
    lcd.print("SMS Received.");
    delay(1000);
    digitalWrite(pump, HIGH);
    message = "Pump is turned OFF";
    lcd.clear();
    lcd.print(message);
    delay(1000);
    lcd.clear();
    lcd.print("Sending SMS.");
    send_message(message);
    delay(1000);
    lcd.clear();
  } 
  // if received command is to status
  if(incomingData.indexOf("status")>=0)
  {
    lcd.clear();
    lcd.print("SMS Received.");
    delay(1000);
    message = "Moisture = ";
    if(digitalRead(sensor))
    message += "Need Water";
    else
    message += "OK";  
    
    lcd.clear();
    lcd.print(message);
    delay(1000);
    lcd.clear();
    lcd.print("Sending SMS.");
    send_message(message);
    delay(1000);
    lcd.clear();
  }        
}

void receive_message()
{
  if (SIM800.available() > 0)
  {
    incomingData = SIM800.readString(); // Get the data from the serial port.
    Serial.print(incomingData); 
    delay(10); 
  }
}

void send_message(String message)
{
  SIM800.println("AT+CMGF=1");    //Set the GSM Module in Text Mode
  delay(100);  
  SIM800.println("AT+CMGS=\"+8801612431905\""); // Replace it with your mobile number
  delay(100);
  SIM800.println(message);   // The SMS text you want to send
  delay(100);
  SIM800.println((char)26);  // ASCII code of CTRL+Z
  delay(100);
  SIM800.println();
  delay(1000);  
}
