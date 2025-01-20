#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
const int Moister_sensor_pin = A0;
const int PUMP_PIN = 3;
int s = 0;
void setup() {  
  Serial.begin(9600);
  pinMode(PUMP_PIN,OUTPUT);
  digitalWrite(PUMP_PIN, HIGH);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("  WELCOME");
}

void loop() {  
  delay(2000);
  lcd.clear();
  lcd.print("Smart Irrigation"); 
  int moister = analogRead(Moister_sensor_pin); 
  lcd.setCursor(0,1);
  lcd.print("Moister: ");
  lcd.print(moister);
    if(moister > 500){
      digitalWrite(PUMP_PIN, LOW);
    }
    else{
      digitalWrite(PUMP_PIN, HIGH);
    }
}
