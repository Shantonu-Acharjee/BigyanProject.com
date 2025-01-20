#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
#define solar_PIN A0
#define wind_PIN A1
#define main_PIN A2
#define solar_switch 5
#define wind_switch 4
#define main_switch 3
float solar_voltage = 0.0;
float wind_voltage = 0.0;
float main_voltage = 0.0;
float R1 = 30000.0;
float R2 = 7500.0;
float ref_voltage = 5.0;
int solar_value = 0;
int wind_value = 0;
int main_value = 0;
void setup() {
  Serial.begin(9600);
  pinMode(solar_switch,OUTPUT);
  pinMode(wind_switch,OUTPUT);
  pinMode(main_switch,OUTPUT);
  digitalWrite(solar_switch, HIGH);
  digitalWrite(wind_switch, HIGH);
  digitalWrite(main_switch, HIGH);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(1,0);
  lcd.print("Welcome To DC");
  lcd.setCursor(0,1);
  lcd.print("Smart MicroGrid");
  delay(5000);
}

void loop() {
  lcd.clear();
  solar_value = analogRead(solar_PIN);
  wind_value = analogRead(wind_PIN);
  main_value = analogRead(main_PIN);
  
  solar_voltage  = (solar_value * ref_voltage) / 1024.0;
  wind_voltage  = (wind_value * ref_voltage) / 1024.0;
  main_voltage  = (main_value * ref_voltage) / 1024.0;
  
  // Calculate voltage at divider input
  solar_voltage = solar_voltage*(R1+R2)/R2;
  wind_voltage = wind_voltage*(R1+R2)/R2;
  main_voltage = main_voltage*(R1+R2)/R2;
  
  // Print results to Serial Monitor to 2 decimal places
  Serial.print("Input Voltage = ");
  Serial.print(solar_voltage, 2);
  Serial.print(", ");
  Serial.print(wind_voltage, 2);
  Serial.print(", ");
  Serial.println(main_voltage, 2);
  lcd.setCursor(0,0);
  lcd.print("So=");
  lcd.print(solar_voltage,0);
  lcd.print(" Wi=");
  lcd.print(wind_voltage,0);
  lcd.print(" Ma=");
  lcd.print(main_voltage,0);

  if((int)solar_voltage >= 2){
    digitalWrite(solar_switch, LOW);
    digitalWrite(wind_switch, HIGH);
    digitalWrite(main_switch, HIGH);
    lcd.setCursor(0,1);
    lcd.print("Solar Line Active");
  }
  else if((int)wind_voltage >= 2){
    digitalWrite(solar_switch, HIGH);
    digitalWrite(wind_switch, LOW);
    digitalWrite(main_switch, HIGH);
    lcd.setCursor(0,1);
    lcd.print("Wind Line Active");
  }
  else if((int)main_voltage >= 2){
    digitalWrite(solar_switch, HIGH);
    digitalWrite(wind_switch, HIGH);
    digitalWrite(main_switch, LOW);
    lcd.setCursor(0,1);
    lcd.print("Main Line Active");
  }
  
  // Short delay
  delay(500);

}
