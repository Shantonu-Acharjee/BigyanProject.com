#include "DHT.h"
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);
Servo myservo;

#define REPORTING_PERIOD_MS     1000
#define DHTTYPE DHT22
#define SmokePIN A1
#define CarbonPIN A0
#define Load1PIN 13
#define Load2PIN 12
#define Load3PIN 11
#define Load4PIN 10
#define DHTPIN 9
#define RainPIN 8
#define MotionPIN 7
#define ServoPIN 6
#define Input1PIN 5
#define Input2PIN 4
#define Input3PIN 3
#define Input4PIN 2
#define BuzzerPIN 16

int windowFlag = 0;
int pos = 0;    // variable to store the servo position
uint32_t tsLastReport = 0;

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);  
  pinMode(Load1PIN, OUTPUT);
  pinMode(Load2PIN, OUTPUT);
  pinMode(Load3PIN, OUTPUT);
  pinMode(Load4PIN, OUTPUT);
  pinMode(BuzzerPIN, OUTPUT);
  pinMode(RainPIN, INPUT);
  pinMode(MotionPIN, INPUT);
  pinMode(Input1PIN, INPUT_PULLUP);
  pinMode(Input2PIN, INPUT_PULLUP);
  pinMode(Input3PIN, INPUT_PULLUP);
  pinMode(Input4PIN, INPUT_PULLUP);
  myservo.attach(ServoPIN);
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  dht.begin();
  lcd.setCursor(6,0);
  lcd.print("WELLCOME");
  lcd.setCursor(9,1);
  lcd.print("TO");
  lcd.setCursor(4,2);
  lcd.print("SMART OFFICE");
  delay(2000);
  myservo.write(100);
}

void loop() {
  int smoke = analogRead(A1);
  int carbon = analogRead(A0);
  int rainStatus = digitalRead(RainPIN);
  int motionStatus = digitalRead(MotionPIN);
  digitalWrite(Load2PIN,digitalRead(Input2PIN));
  digitalWrite(Load3PIN,digitalRead(Input3PIN));
  digitalWrite(Load1PIN,digitalRead(Input1PIN));   
  if(motionStatus == 1){
    digitalWrite(Load4PIN, LOW);
  }
  else{
    digitalWrite(Load4PIN, HIGH); 
  }
  if(smoke > 300){
    digitalWrite(BuzzerPIN, LOW);
  }
  else{
    digitalWrite(BuzzerPIN, HIGH);    
  }
  
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    int h = dht.readHumidity();
    int t = dht.readTemperature();
    if (isnan(h) || isnan(t)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }  
    lcd.clear();
    lcd.setCursor(4,0);
    lcd.print("SMART OFFICE");
    lcd.setCursor(0,1);
    lcd.print("Humi:");
    lcd.print(h);
    lcd.print("%");
    lcd.print(" Temp:");
    lcd.print(t);
    lcd.print(" C");
    lcd.setCursor(0,2);
    lcd.print("Smoke:");
    lcd.print(smoke);
    lcd.print(" Carbon:");
    lcd.print(carbon);
    lcd.setCursor(0,3);
    lcd.print("Rain:");
    if(rainStatus == 1){
      lcd.print(" No");
    }
    else{
      lcd.print(" Yes");
    }
    lcd.print(" Motion:");
    if(motionStatus == 0){
      lcd.print(" No");
    }
    else{
      lcd.print(" Yes");
    }
    String str =String(h)+String("=")+String(t)+String("=")+String(smoke)+String("=")+String(carbon)+String("=")+String(rainStatus)+String("=")+String(motionStatus);
    Serial.println(str);
    if(rainStatus == 0 && windowFlag == 0)
    {
      for (pos = 100; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(15);                       // waits 15ms for the servo to reach the position
      }    
      windowFlag = 1;
    }
    else if(rainStatus == 1 && windowFlag == 1)
    {
      for (pos = 0; pos <= 100; pos += 1) { // goes from 0 degrees to 180 degrees
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(15);                       // waits 15ms for the servo to reach the position
      }
      windowFlag = 0;
    }
    tsLastReport = millis();
  }
}
