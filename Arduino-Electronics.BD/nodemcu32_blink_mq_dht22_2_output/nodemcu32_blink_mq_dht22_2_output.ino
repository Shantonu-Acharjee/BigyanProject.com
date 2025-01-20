#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#define GASPIN 34
#define FANPIN 13
#define LIGHTPIN 14
#include "esp_adc_cal.h"
 
#define LM35_Sensor1    35
 
int LM35_Raw_Sensor1 = 0;
float LM35_TempC_Sensor1 = 0.0;
float LM35_TempF_Sensor1 = 0.0;
float Voltage = 0.0;
char auth[] = "kQfXCivNrOIbhTNExc35-XXDCfeOag0T";

char ssid[] = "Salman";
char pass[] = "salman789";

int RelayPinState0 = HIGH;        // OFF
int RelayPinState1 = HIGH;        // OFF

BlynkTimer timer;
void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  pinMode(FANPIN, OUTPUT);
  pinMode(LIGHTPIN, OUTPUT);
  timer.setInterval(1000L, sendSensor);
}

void loop() {
  Blynk.run();
  timer.run();
  // Read LM35_Sensor1 ADC Pin
  LM35_Raw_Sensor1 = analogRead(LM35_Sensor1);  
  // Calibrate ADC & Get Voltage (in mV)
  Voltage = readADC_Cal(LM35_Raw_Sensor1);
  // TempC = Voltage(mV) / 10
  LM35_TempC_Sensor1 = Voltage / 10;
  LM35_TempF_Sensor1 = (LM35_TempC_Sensor1 * 1.8) + 32;
}
BLYNK_CONNECTED()
{
  // Request the latest state from the server
  Blynk.syncVirtual(V0);
  Blynk.syncVirtual(V1);
}
BLYNK_WRITE(V0)
{                           
  RelayPinState0 = param.asInt();
  digitalWrite(FANPIN, RelayPinState0);
}
BLYNK_WRITE(V1)
{                           
  RelayPinState1 = param.asInt();
  digitalWrite(LIGHTPIN, RelayPinState1);
}
void sendSensor()
{
  int g = analogRead(GASPIN);
 
  // Print The Readings
  Serial.print("Temperature = ");
  Serial.print(LM35_TempC_Sensor1);
  Serial.print(" °C , ");
  Blynk.virtualWrite(V3, LM35_TempC_Sensor1);
  Blynk.virtualWrite(V4, g);
  Serial.println(g);
}
uint32_t readADC_Cal(int ADC_Raw)
{
  esp_adc_cal_characteristics_t adc_chars;
  
  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);
  return(esp_adc_cal_raw_to_voltage(ADC_Raw, &adc_chars));
}
