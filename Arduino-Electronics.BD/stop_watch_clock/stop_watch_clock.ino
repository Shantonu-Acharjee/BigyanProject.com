#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "RTClib.h"

// button variables
const int modeBtn = 10;
const int startBtn = 11;
const int resetBtn = 12;
int startBtnState = 0;



int cflag = 0;
int sflag = 0;
int h = 0;
int m = 0;
int s = 0;

// clock setup
RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
LiquidCrystal_I2C lcd(0x27,16,2);

void setup()
{
  pinMode(modeBtn, INPUT_PULLUP);
  pinMode(startBtn, INPUT_PULLUP);
  pinMode(resetBtn, INPUT_PULLUP);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

//For changing time
    
    //rtc.adjust(DateTime(2017, 12, 7, 20, 41, 0));
   
    //if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    //rtc.adjust(DateTime(2017, 12, 7, 20, 36, 0));
  //}
  lcd.init();
  lcd.backlight();
}

void loop() 
{
  if (digitalRead(modeBtn) == LOW) {
    if(cflag ==0){
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
      cflag = 1;
      sflag = 0;
      startBtnState = 0;
      h = 0;
      m = 0;
      s = 0;
      lcd.clear();
    }
    DateTime now = rtc.now();
    // day
    if(now.day() < 10){
      lcd.setCursor(3,0);
      lcd.print("0");
      lcd.setCursor(4,0);
      lcd.print(now.day(), DEC);
      lcd.print("/");
    }
    else{
      lcd.setCursor(3,0);
      lcd.print(now.day(), DEC);
      lcd.print("/");
    }
    
    // month
    if(now.month() < 10){
      lcd.setCursor(6,0);
      lcd.print("0");
      lcd.setCursor(7,0);
      lcd.print(now.month(), DEC);
      lcd.print("/");
    }
    else{
      lcd.setCursor(6,0);
      lcd.print(now.month(), DEC);
      lcd.print("/");
    }
    // year
    lcd.setCursor(9,0);
    lcd.print(now.year(), DEC);
    
    //time
    
    // hour
    if(now.hour() < 10){
      lcd.setCursor(4,1);
      lcd.print("0");
      lcd.setCursor(5,1);
      lcd.print(now.hour(), DEC);
      lcd.print(":");
    }
    else{
      lcd.setCursor(4,1);
      lcd.print(now.hour(), DEC);
      lcd.print(":");
    }
    
    // minute
    if(now.minute() < 10){
      lcd.setCursor(7,1);
      lcd.print("0");
      lcd.setCursor(8,1);
      lcd.print(now.minute(), DEC);
      lcd.print(".");
    }    
    else {
      lcd.setCursor(7,1);
      lcd.print(now.minute(), DEC);
      lcd.print(":");
    }
    
    // second
    if(now.second() < 10){
      lcd.setCursor(10,1);
      lcd.print("0");
      lcd.setCursor(11,1);
      lcd.print(now.second(), DEC);
    }
    else{
      lcd.setCursor(10,1);
      lcd.print(now.second(), DEC);
    }

  // Stopwatch
  }
  else {
    if(sflag ==0){
      rtc.adjust(DateTime(2021, 1, 1, 0, 0, 0));
      sflag = 1;
      cflag = 0;
      startBtnState = 0;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("STOPWATCH MODE");
      lcd.setCursor(0,1);
      lcd.print("PRESS BUTTON");
    }
    // hour
    if(digitalRead(startBtn) == LOW){
      if(startBtnState == 0){
        startBtnState = 1;
        rtc.adjust(DateTime(2021, 01, 01, h, m, s));
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("STOPWATCH");
      }
      else{
        startBtnState = 0;
      }
    }
    if(digitalRead(resetBtn) == LOW){
      startBtnState = 0;
      rtc.adjust(DateTime(2021, 01, 01, 0, 0, 0));
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("STOPWATCH");
      h = 0;
      m = 0;
      s = 0;
    }
    if(startBtnState == 1){      
      DateTime now = rtc.now();
      h = now.hour();
      m = now.minute();
      s = now.second();
      if(now.hour() < 10){
        lcd.setCursor(0,1);
        lcd.print("0");
        lcd.setCursor(1,1);
        lcd.print(now.hour(), DEC);
        lcd.print(":");
      }
      else{
        lcd.setCursor(0,1);
        lcd.print(now.hour(), DEC);
        lcd.print(":");
      }
      
      // minute
      if(now.minute() < 10){
        lcd.setCursor(3,1);
        lcd.print("0");
        lcd.setCursor(4,1);
        lcd.print(now.minute(), DEC);
        lcd.print(".");
      }    
      else {
        lcd.setCursor(3,1);
        lcd.print(now.minute(), DEC);
        lcd.print(":");
      }
      
      // second
      if(now.second() < 10){
        lcd.setCursor(6,1);
        lcd.print("0");
        lcd.setCursor(7,1);
        lcd.print(now.second(), DEC);
      }
      else{
        lcd.setCursor(6,1);
        lcd.print(now.second(), DEC);
      }
    }
  }
  //lcd.clear();
}
