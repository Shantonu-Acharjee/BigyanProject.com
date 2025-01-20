#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>
#define REPORTING_PERIOD_MS     1000
#define AVG_NUM 50    // number of iterations of the adc routine to average the adc readings
#define SOL_ADC A0     // Solar panel side voltage divider is connected to pin A0 
#define CURRENT_ADC A1  // ACS 712 current sensor is connected to pin A2
SoftwareSerial nodeMcu(4,3); // (Rx, Tx)
LiquidCrystal_I2C lcd(0x27,20,4);
float solar_volt = 0;
float load_current = 0;
float msec = 0;
float last_msec = 0;
float elasped_msec = 0;
float elasped_time = 0;
float ampSecs = 0;
float ampHours = 0;
float watts = 0;
float wattSecs = 0;
float wattHours = 0;
uint32_t tsLastReport = 0;
String str;
void setup() {
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  digitalWrite(5,HIGH);
  digitalWrite(6,LOW);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  digitalWrite(8,HIGH);
  digitalWrite(7,LOW);
  Serial.begin(115200);
  nodeMcu.begin(115200);
  lcd.init();
  lcd.backlight();
}

void loop() {
  read_data();             // read different sensors data from analog pin of arduino
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {      
      lcd.clear();
      Serial.print("Voltage:");
      Serial.print(solar_volt);
      Serial.print("V / Cuttent:");
      Serial.print(load_current);
      Serial.print("mA");
      Serial.print(" Power:  ");
      Serial.print(watts);
      Serial.print(" Wh: ");
      Serial.println(wattHours);     
      lcd.setCursor(0,0);
      lcd.print("Vo:");
      lcd.print(solar_volt);
      lcd.print(" Cu:");
      lcd.print(load_current);
      lcd.print("a");
      lcd.setCursor(0,1);
      lcd.print("Pw:");
      lcd.print(watts);
      lcd.print(" En:");
      lcd.print(wattHours);
      str =String(solar_volt)+String("=")+String(load_current)+String("=")+String(watts)+String("=")+String(wattHours);
      nodeMcu.println(str);
      Serial.println("Json Sent");
      tsLastReport = millis();
  }
}
//-------------------------------------------------------------------------------------------------------------
////////////////////////////////////READ THE DATA//////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------------------
void read_data(void)
{
  //5V = ADC value 1024 => 1 ADC value = (5/1024)Volt= 0.0048828Volt
  // Vout=Vin*R2/(R1+R2) => Vin = Vout*(R1+R2)/R2   R1=30100 and R2=7500
  solar_volt = (read_adc(SOL_ADC) * 0.00488)/0.1995;
  load_current = (read_adc(CURRENT_ADC) * 0.00488 - 2.47)/0.185;
  msec = millis();
  elasped_msec = msec - last_msec; //Calculate how long has past since last call of this function
  elasped_time = elasped_msec / 1000.0; // 1sec=1000 msec
  watts = load_current * solar_volt; //Watts now
  ampSecs = (load_current * elasped_time); //AmpSecs since last measurement
  wattSecs = ampSecs * solar_volt; //WattSecs since last measurement
  ampHours = ampHours + ampSecs / 3600; // 1 hour=3600sec //Total ampHours since program started
  wattHours = wattHours + wattSecs / 3600; // 1 hour=3600sec //Total wattHours since program started
  last_msec = msec; //Store 'now' for next time
}

int read_adc(int adc_parameter)
{

  int sum = 0;
  int sample ;
  for (int i = 0; i < AVG_NUM; i++)
  { // loop through reading raw adc values AVG_NUM number of times
    sample = analogRead(adc_parameter);    // read the input pin
    sum += sample;                        // store sum for averaging
    delayMicroseconds(50);              // pauses for 50 microseconds
  }
  return (sum / AVG_NUM);               // divide sum by AVG_NUM to get average and return it
}
