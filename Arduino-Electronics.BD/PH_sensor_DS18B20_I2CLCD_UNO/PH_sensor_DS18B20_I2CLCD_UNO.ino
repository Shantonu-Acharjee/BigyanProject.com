#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define sensorPin A1
#define ONE_WIRE_BUS 14
float calibration_value = 23.03;
int phval = 0; 
unsigned long int avgval; 
int buffer_arr[10],temp;
int val = 0;

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

 float Celcius=0;
void setup() {
  Serial.begin(9600);  
  sensors.begin();
  lcd.init();
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("   Welcome   ");
  lcd.setCursor(0, 1);
  lcd.print("");
  delay(2000);
  lcd.clear();
}

void loop(void)
{   
  sensors.requestTemperatures(); 
  Celcius=sensors.getTempCByIndex(0);
  Serial.print(" C  ");
  Serial.print(Celcius);
  for(int i=0;i<10;i++) 
  { 
    buffer_arr[i]=analogRead(A1);
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
  float ph_act = -3.70 * volt + calibration_value;
  Serial.print(" pH Val:  ");
  Serial.println(ph_act);  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("pH Val:");
  lcd.setCursor(8, 0);
  lcd.print(ph_act);
  lcd.setCursor(0, 1);
  lcd.print("Temp:");
  lcd.setCursor(8, 1);
  lcd.print(Celcius);
  lcd.print(" C");
  delay(1000);
}
