#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
#define IN_11  4    // L298N #1 in 1 motor Right Ahead
#define IN_12  5    // L298N #1 in 2 motor Right Ahead
const int Moister_sensor_pin = 6;
const int PUMP_PIN = 7;
const int Mode_PIN = 8;
int s = 0;
void setup() {  
  Serial.begin(115200);
  pinMode(Moister_sensor_pin,INPUT);
  pinMode(PUMP_PIN,INPUT);
  pinMode(Mode_PIN,INPUT);
  pinMode(IN_11, OUTPUT);
  pinMode(IN_12, OUTPUT);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("  WELCOME");
}

void loop() {  
  delay(2000);
  lcd.clear();
  lcd.print(" IOT Irrigation");
  if(!digitalRead(Mode_PIN)){
    
    if(digitalRead(Moister_sensor_pin)){
      digitalWrite(IN_11, HIGH);
      digitalWrite(IN_12, LOW);
      lcd.setCursor(0,1);
      lcd.print("PUMP ON");
      s = 1;
    }
    else{
      digitalWrite(IN_11, LOW);
      digitalWrite(IN_12, LOW);
      lcd.setCursor(0,1);
      lcd.print("PUMP OFF");
      s = 0;
    }
  }
  else{    
    if(digitalRead(PUMP_PIN)){
      digitalWrite(IN_11, HIGH);
      digitalWrite(IN_12, LOW);
      lcd.setCursor(0,1);
      lcd.print("PUMP ON");
      s = 1;
    }
    else{
      
      digitalWrite(IN_11, LOW);
      digitalWrite(IN_12, LOW);
      lcd.setCursor(0,1);
      lcd.print("PUMP OFF");
      s = 0;
    }
  }
  String str =String(s);
  Serial.print(str);
}
