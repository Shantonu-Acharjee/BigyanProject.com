#include <Servo.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
Servo MetalServo;
Servo GripServo;
Servo ElboServo;
const int s0 = 7;  
const int s1 = 8;  
const int s2 = 10;  
const int s3 = 11;  
const int out = 12; 
const int motor = 13; 
const int MetalSensor = 4; 
const int MetalBoxSensor = 3; 
const int BoxSensor = 2;  
const int ColorBoxSensor = 14;  
int metalFlag = 0; 
int colorFlag = 0;
int MetalServopos = 90;
int GripServopos = 5;
int ElboServopos = 180;
int red = 0;  
int green = 0;  
int blue = 0;
int redCount = 0;  
int greenCount = 0;  
int blueCount = 0;
void setup() {
  Serial.begin(9600);
  pinMode(motor,OUTPUT);
  pinMode(MetalBoxSensor,INPUT);
  pinMode(BoxSensor,INPUT);
  pinMode(ColorBoxSensor,INPUT);
  pinMode(s0, OUTPUT);  
  pinMode(s1, OUTPUT);  
  pinMode(s2, OUTPUT);  
  pinMode(s3, OUTPUT);  
  pinMode(out, INPUT);   
  digitalWrite(s0, HIGH);  
  digitalWrite(s1, HIGH);
  MetalServo.attach(5); 
  GripServo.attach(6); 
  ElboServo.attach(9); 
  MetalServo.write(MetalServopos);
  GripServo.write(GripServopos);
  ElboServo.write(ElboServopos);
  digitalWrite(motor,HIGH);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(1,0);
  lcd.print("   Welcome");
  delay(5000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("RED  GREEN  BLUE");
  lcd.setCursor(1,1);
  lcd.print(redCount);
  lcd.setCursor(7,1);
  lcd.print(greenCount);
  lcd.setCursor(14,1);
  lcd.print(blueCount);
  digitalWrite(motor,LOW);
}
void loop() {
  int Ismetal = digitalRead(MetalSensor);
  //Serial.print("Metal=");
  //Serial.print(Ismetal);
  if(Ismetal == HIGH || metalFlag == 1){
    metalFlag = 1;
    int IsBoxPositioned = digitalRead(MetalBoxSensor);
    Serial.print("Box=");
    Serial.print(IsBoxPositioned);
    if(IsBoxPositioned == LOW){
      digitalWrite(motor,HIGH);
      for (MetalServopos = 90; MetalServopos >= 40; MetalServopos -= 1) { // goes from 180 degrees to 0 degrees
        MetalServo.write(MetalServopos);              // tell servo to go to position in variable 'pos'
        delay(5);                       // waits 15 ms for the servo to reach the position
      }
      for (MetalServopos = 40; MetalServopos <= 90; MetalServopos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
        MetalServo.write(MetalServopos);              // tell servo to go to position in variable 'pos'
        delay(5);                       // waits 15 ms for the servo to reach the position
      }
      metalFlag = 0;
      digitalWrite(motor,LOW);
    }
  }
  else{
    int IsBoxPositioned = digitalRead(ColorBoxSensor);
    //Serial.print("Box=");
    //Serial.print(IsBoxPositioned);
    if(IsBoxPositioned == LOW){
      delay(300);
      color(); 
      Serial.print("R Intensity:");  
      Serial.print(red, DEC);  
      Serial.print(" G Intensity: ");  
      Serial.print(green, DEC);  
      Serial.print(" B Intensity : ");  
      Serial.print(blue, DEC);
      Serial.println();
      if (red < blue && red < green && red < 40)
      {  
        Serial.println(" - (Red Color)"); 
        int IsBoxPositioned = digitalRead(BoxSensor); 
        while(IsBoxPositioned == HIGH) {
          IsBoxPositioned = digitalRead(BoxSensor);
          delay(5);
        }
        digitalWrite(motor,HIGH);
        gripclose();
        moveelbo(1);
        digitalWrite(motor,LOW);
      } 
      else if (blue < red && blue < green)   
      {  
        if(blue < 80) {
          Serial.println(" - (Blue Color)"); 
          int IsBoxPositioned = digitalRead(BoxSensor); 
          while(IsBoxPositioned == HIGH) {
            IsBoxPositioned = digitalRead(BoxSensor);
            delay(5);
          }
          digitalWrite(motor,HIGH);
          gripclose();
          moveelbo(2); 
          digitalWrite(motor,LOW);
        }
        else{
          //Serial.println(" - (NO Color)"); 
        }   
      }
      else if (green < red && green < blue)  
      {  
        Serial.println(" - (Green Color)");  
        int IsBoxPositioned = digitalRead(BoxSensor); 
          while(IsBoxPositioned == HIGH) {
            IsBoxPositioned = digitalRead(BoxSensor);
            delay(5);
          }
          digitalWrite(motor,HIGH);
          gripclose();
          moveelbo(3);         
          digitalWrite(motor,LOW);
      } 
    }
  }
}
void gripopen(){
        Serial.println("open");  
  for (GripServopos = 40; GripServopos >= 0; GripServopos -= 1) { // goes from 40 degrees to 5 degrees
    GripServo.write(GripServopos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15 ms for the servo to reach the position
  }
}
void gripclose(){
  
        Serial.println("close");  
  for (GripServopos = 0; GripServopos <= 40; GripServopos += 1) { // goes from 5 degrees to 40 degrees
// in steps of 1 degree
    GripServo.write(GripServopos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15 ms for the servo to reach the position
  }
}
void moveelbo(int position){
  if(position == 1){
    for (ElboServopos = 180; ElboServopos >= 0; ElboServopos -= 1) { // goes from 40 degrees to 5 degrees
      ElboServo.write(ElboServopos);              // tell servo to go to position in variable 'pos'
      delay(5);                       // waits 15 ms for the servo to reach the position
    }
    redCount++;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("RED  GREEN  BLUE");
    lcd.setCursor(1,1);
    lcd.print(redCount);
    lcd.setCursor(7,1);
    lcd.print(greenCount);
    lcd.setCursor(14,1);
    lcd.print(blueCount);
  }
  else if(position == 2){
    for (ElboServopos = 180; ElboServopos >= 40; ElboServopos -= 1) { // goes from 40 degrees to 5 degrees
      ElboServo.write(ElboServopos);              // tell servo to go to position in variable 'pos'
      delay(5);                       // waits 15 ms for the servo to reach the position
    }
    blueCount++;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("RED  GREEN  BLUE");
    lcd.setCursor(1,1);
    lcd.print(redCount);
    lcd.setCursor(7,1);
    lcd.print(greenCount);
    lcd.setCursor(14,1);
    lcd.print(blueCount);
  }
  else if(position == 3){
    for (ElboServopos = 180; ElboServopos >= 80; ElboServopos -= 1) { // goes from 40 degrees to 5 degrees
      ElboServo.write(ElboServopos);              // tell servo to go to position in variable 'pos'
      delay(5);                       // waits 15 ms for the servo to reach the position
    }
    greenCount++;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("RED  GREEN  BLUE");
    lcd.setCursor(1,1);
    lcd.print(redCount);
    lcd.setCursor(7,1);
    lcd.print(greenCount);
    lcd.setCursor(14,1);
    lcd.print(blueCount);
  }
  gripopen();
  for (ElboServopos = ElboServopos; ElboServopos <= 180; ElboServopos += 1) { // goes from 5 degrees to 40 degrees
// in steps of 1 degree
    ElboServo.write(ElboServopos);              // tell servo to go to position in variable 'pos'
    delay(5);                       // waits 15 ms for the servo to reach the position
  }
}
void color()  
{    
  digitalWrite(s2, LOW);  
  digitalWrite(s3, LOW);  
  //count OUT, pRed, RED  
  red = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);  
  digitalWrite(s3, HIGH);  
  //count OUT, pBLUE, BLUE  
  blue = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);  
  digitalWrite(s2, HIGH);  
  //count OUT, pGreen, GREEN  
  green = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);  
}