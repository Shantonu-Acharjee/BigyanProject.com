#include <LiquidCrystal.h>
#include <Servo.h>
 
Servo myservo;  // create servo object to control a servo
                // a maximum of eight servo objects can be created
 
int pos = 0;    // variable to store the servo position
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
#define PIR_sensor 14
void setup() 
{
  myservo.attach(6);
  lcd.begin(16, 2);
  pinMode(PIR_sensor, INPUT);
  lcd.print("    Automatic    ");
  lcd.setCursor(0,1); 
  lcd.print("   Door Opener   ");
  delay(3000);
  lcd.clear();
  lcd.print("Electronics.Com");
  delay(2000);
}
void loop() 
{
  if(digitalRead(PIR_sensor))
  {
    lcd.setCursor(0,0);
    lcd.print("Movement Detected");
    lcd.setCursor(0, 1);
    lcd.print("  Gate Opened    ");
    for(pos = 0; pos < 180; pos += 1)  // goes from 0 degrees to 180 degrees
    {                                  // in steps of 1 degree
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
    delay(2000);
    lcd.clear();
    lcd.print("   Gate Closed    ");
    for(pos = 180; pos>=1; pos-=1)     // goes from 180 degrees to 0 degrees
    {
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
    delay(2000);
  }
  
  else 
  {
    lcd.setCursor(0,0);
    lcd.print("   No Movement   ");
    lcd.setCursor(0,1);
    lcd.print("   Gate Closed   ");
    myservo.write(0);
  }
}
