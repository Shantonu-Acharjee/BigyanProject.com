#include <LiquidCrystal_I2C.h>
#define BLYNK_TEMPLATE_ID "TMPLoXgwOkIR"
#define BLYNK_TEMPLATE_NAME "IOT Irrigation System"
#define BLYNK_AUTH_TOKEN "EnL2QM3LpSaBtxZLzW3WaK7eRMa9PL6f"
LiquidCrystal_I2C lcd(0x27, 16,2);
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).

char auth[] = "EnL2QM3LpSaBtxZLzW3WaK7eRMa9PL6f";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Sarah";
char pass[] = "abcd1234efgh";
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "DHT.h"
#define DHTPIN 4
#define RAINPIN 16
#define FANPIN 25
#define PUMPPIN 26
#define LEDPIN 33
#define BUZZARPIN 23
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;
int manualOff = 0;
BLYNK_WRITE(V4)
{
  if(param.asInt()==1){
    digitalWrite(PUMPPIN, HIGH);
    manualOff = 1;
  }
  else{
    digitalWrite(PUMPPIN, LOW);
    manualOff = 0;
  }
}

BLYNK_CONNECTED()
{
  Blynk.syncVirtual(V0);  

  Blynk.syncVirtual(V1);  

  Blynk.syncVirtual(V2);  

  Blynk.syncVirtual(V3); 
  
  Blynk.syncVirtual(V4);  
}
void setup()
{
  // Debug console
  Serial.begin(115200);
  dht.begin();
  pinMode(RAINPIN, INPUT);
  pinMode(FANPIN, OUTPUT);
  pinMode(PUMPPIN, OUTPUT);
  pinMode(LEDPIN, OUTPUT);
  pinMode(BUZZARPIN, OUTPUT);
  digitalWrite(LEDPIN, LOW);
  digitalWrite(FANPIN, HIGH);
  digitalWrite(PUMPPIN, HIGH);
  digitalWrite(BUZZARPIN, HIGH);
  lcd.init();
  lcd.begin(16, 2);
  
  // Turn on the backlight on LCD. 
  lcd.backlight();
  lcd.setCursor(0, 0);
  // print the Message on the LCD. 
  lcd.print( " IOT Irrigation" );
  lcd.setCursor(0, 1);
  // print the Message on the LCD. 
  lcd.print( "    System" );
  timer.setInterval(2000L, sensorValue);
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  Blynk.run();
  timer.run();
}

void sensorValue(){
  int h = dht.readHumidity();
  int t = dht.readTemperature();
  if (isnan(h)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  int m = ( 100 - ( (analogRead(36)/4095.00) * 100 ) );
  int l = analogRead(39);
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F(" Moister: "));
  Serial.print(m);
  Serial.print(F(" Light: "));
  Serial.print(l);
  Serial.println();
  lcd.clear();
  lcd.setCursor(0, 0);
  // print the Message on the LCD. 
  lcd.print( "Hum: " );
  lcd.print( h );
  lcd.print( " Tem:" );
  lcd.print( t );
  lcd.setCursor(0, 1);
  lcd.print( "Moister: " );
  lcd.print( m );
  Blynk.virtualWrite(V0, m);
  Blynk.virtualWrite(V1, h);
  Blynk.virtualWrite(V2, t);
  if(l > 2000)
  {
    digitalWrite(LEDPIN, LOW);
  }
  else{
    digitalWrite(LEDPIN, HIGH);
  }
  if(t > 30)
  {
    digitalWrite(FANPIN, LOW);
  }
  else{
    digitalWrite(FANPIN, HIGH);
  }
  if(m < 30)
  {
    digitalWrite(PUMPPIN, LOW);
    digitalWrite(BUZZARPIN, LOW);
  }
  else if(m > 60){
    digitalWrite(PUMPPIN, HIGH);
    digitalWrite(BUZZARPIN, HIGH);
  }
  if(digitalRead(RAINPIN)){
    Blynk.virtualWrite(V3, "Rain Stop");
  }
  else{
    Blynk.virtualWrite(V3, "Rain Start");
  }
}
