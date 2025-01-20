
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
LiquidCrystal_I2C lcd(0x27,16,2);
float lattitude,longitude; // create variable for latitude and longitude object 
SoftwareSerial gpsSerial(9,8);//rx,tx
TinyGPSPlus gps;// create gps object
int switch_pin 2;
void setup(){
  pinMode(switch_pin,INPUT_PULLUP);
Serial.begin(9600); // connect serial
lcd.init();
gpsSerial.begin(9600); // connect gps sensor
lcd.backlight();
lcd.clear();
}
void loop(){
  
  while(1)
  {
   while (gpsSerial.available() > 0)
   { gps.encode(gpsSerial.read()); }

      if (gps.location.isUpdated())
      {
       Serial.print("LAT=");  Serial.println(gps.location.lat(), 6);
       Serial.print("LONG="); Serial.println(gps.location.lng(), 6);
       lattitude=gps.location.lat();
       longitude=gps.location.lng();
      break;
      }
  
   
  
}

 Serial.print("LATTITUDE="); Serial.println(lattitude,6);
 Serial.print("LONGITUDE="); Serial.println(longitude,6);
 lcd.print("LAT ");lcd.print(lattitude,6);
 lcd.setCursor(0, 1);
 lcd.print("LONG ");lcd.print(longitude,6);
 delay(1000);
 lcd.clear();

}
