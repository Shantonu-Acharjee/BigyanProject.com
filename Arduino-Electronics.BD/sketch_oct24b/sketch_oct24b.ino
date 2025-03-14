#include "DHT.h"
//#include <SoftwareSerial.h>
#define DHTPIN 2
// Uncomment whatever type you're using!
//#define DHTTYPE DHT11 // DHT 11
#define DHTTYPE DHT22 // DHT 22 (AM2302), AM2321
//#define DHTTYPE DHT21 // DHT 21 (AM2301)
//SoftwareSerial espSerial(0, 1);
DHT dht(DHTPIN, DHTTYPE);
String str;
void setup(){
Serial.begin(115200);
//espSerial.begin(115200);
dht.begin();
delay(2000);
}
void loop()
{
float h = dht.readHumidity();
// Read temperature as Celsius (the default)
float t = dht.readTemperature();
//Serial.print("H: ");
//Serial.print(h);
//Serial.print("% ");
//Serial.print(" T: ");
//Serial.print(t);
//Serial.println("C");
str =String("coming from arduino: ")+String("H= ")+String(h)+String("T= ")+String(t);
Serial.println(str);
delay(1000);
}
