#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPLpp1SP4pi"
#define BLYNK_TEMPLATE_NAME "IoT Patient Monitoring System"
#define BLYNK_AUTH_TOKEN "b1DZm-BVtYlDXePEtbPSQ3n4gb8N8HqD"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>
SoftwareSerial uno(14,12);
BlynkTimer timer;
WidgetLED led1(V6);
#define BLYNK_RED       "#D3435C"
#define BLYNK_GREEN     "#23C48E"
char auth[] = "b1DZm-BVtYlDXePEtbPSQ3n4gb8N8HqD";
char ssid[] = "mobilewifi";
char pass[] = "abcd1234efgh";
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "DHTesp.h"

#define ONE_WIRE_BUS 0
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
#define REPORTING_PERIOD_MS 1000
DHTesp dht;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
uint32_t tsLastReport = 0;
String load = "";
String SpO2 = "";
String bpm = "";
String content = "";
String gaugeColor;
float t;
float h;
float temperatureC;
int ind1; // , locations
int ind2;
int alarm;
const unsigned char bitmap [] PROGMEM=
{
0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x18, 0x00, 0x0f, 0xe0, 0x7f, 0x00, 0x3f, 0xf9, 0xff, 0xc0,
0x7f, 0xf9, 0xff, 0xc0, 0x7f, 0xff, 0xff, 0xe0, 0x7f, 0xff, 0xff, 0xe0, 0xff, 0xff, 0xff, 0xf0,
0xff, 0xf7, 0xff, 0xf0, 0xff, 0xe7, 0xff, 0xf0, 0xff, 0xe7, 0xff, 0xf0, 0x7f, 0xdb, 0xff, 0xe0,
0x7f, 0x9b, 0xff, 0xe0, 0x00, 0x3b, 0xc0, 0x00, 0x3f, 0xf9, 0x9f, 0xc0, 0x3f, 0xfd, 0xbf, 0xc0,
0x1f, 0xfd, 0xbf, 0x80, 0x0f, 0xfd, 0x7f, 0x00, 0x07, 0xfe, 0x7e, 0x00, 0x03, 0xfe, 0xfc, 0x00,
0x01, 0xff, 0xf8, 0x00, 0x00, 0xff, 0xf0, 0x00, 0x00, 0x7f, 0xe0, 0x00, 0x00, 0x3f, 0xc0, 0x00,
0x00, 0x0f, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
void onBeatDetected()
{
  //Serial.println("Beat!");
  display.drawBitmap( 80, 10, bitmap, 28, 28, 1);
  display.display();
}
void setup() {
  Serial.begin(9600);
  uno.begin(9600);
  pinMode(15,INPUT);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SCREEN_ADDRESS, true)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 5);
  display.println(" Welcome to IoT Based");
  display.println("   Portable Cardiac");
  display.println("    Monitoring and");
  display.println("      Problems");
  display.println("    Identification");
  display.println("      Systems.");
  display.display();
  delay(3000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 5);

  display.println("Initializing DHT Sensor..");
  display.display();
  delay(1000);
  dht.setup(13, DHTesp::DHT11);
  display.clearDisplay();
  display.setCursor(0, 5);
  display.println("Initializing Temperature Sensor..");
  display.display();
  delay(1000);
  sensors.begin();
  display.clearDisplay();
  display.setCursor(0, 5);
  display.println("Initializing Online Server..");
  display.display();
  delay(1000);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, sendSensor);
}

void loop() {
  Blynk.run();
  timer.run(); 
  if (uno.available())  {
    char c = uno.read();  //gets one byte from serial buffer
    if (c == '\n') {  
      Serial.println(content);          
      uno.flush();
      ind1 = content.indexOf('=');  //finds location of first ,
      bpm = content.substring(0, ind1);   //captures first data String
      ind2 = content.indexOf('=', ind1+1 );   //finds location of second ,
      load = content.substring(ind1+1, ind2);   //captures second data String
      //ind3 = content.indexOf('=', ind2+1 );
      SpO2 = content.substring(ind2+1);
      load.trim();
      
      if(load.toInt() < 0)
      {
        load = "0";
      }
      content="";
      t = dht.getTemperature();
      h = dht.getHumidity();
      Serial.print(F(" Temperature: "));
      Serial.print(t);
      Blynk.virtualWrite(V2, String(t) );
      Serial.print(F("°C "));
      sensors.requestTemperatures(); 
      temperatureC = DallasTemperature::toFahrenheit(sensors.getTempCByIndex(0));
      Serial.print(temperatureC);
      Serial.println("ºF");
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(SH110X_WHITE);
      display.setCursor(0, 5);
      display.println("BPM");
      display.setTextSize(1);
      display.setTextColor(SH110X_WHITE);
      display.setCursor(30,5);
      display.println(bpm);
      int nbpm = atoi(bpm.c_str());
      if(nbpm <= 40){        
        display.setTextSize(1);
        display.setTextColor(SH110X_WHITE);
        display.setCursor(70, 5);
        display.println("Low Bp");  
      }
      else if(nbpm >= 120){
        display.setTextSize(1);
        display.setTextColor(SH110X_WHITE);
        display.setCursor(70, 5);
        display.println("High Bp"); 
      }
      else{
        display.setTextSize(1);
        display.setTextColor(SH110X_WHITE);
        display.setCursor(70, 5);
        display.println("           "); 
      }
      display.setTextSize(1);
      display.setTextColor(SH110X_WHITE);
      display.setCursor(0, 20);
      display.println("Load");  
      display.setTextSize(1);
      display.setTextColor(SH110X_WHITE);
      display.setCursor(30,20);
      display.println(load);
      
      display.setTextSize(1);
      display.setTextColor(SH110X_WHITE);
      display.setCursor(0, 35);
      display.println("Temp");  
      display.setTextSize(1);
      display.setTextColor(SH110X_WHITE);
      display.setCursor(30,35);
      display.println(temperatureC);

      if(temperatureC > 99){        
        display.setTextSize(1);
        display.setTextColor(SH110X_WHITE);
        display.setCursor(70, 35);
        display.println("Fever");  
      }
      else{
        display.setTextSize(1);
        display.setTextColor(SH110X_WHITE);
        display.setCursor(70, 35);
        display.println("     "); 
      }
      display.setTextSize(1);
      display.setTextColor(SH110X_WHITE);
      display.setCursor(0, 50);
      display.println("Spo2");  
      display.setTextSize(1);
      display.setTextColor(SH110X_WHITE);
      display.setCursor(30,50);
      display.println(SpO2);
      display.display();
      Blynk.virtualWrite(V0, temperatureC);
      Blynk.virtualWrite(V1, h);
      Blynk.virtualWrite(V2, t);
      Blynk.virtualWrite(V3, bpm);
      Blynk.virtualWrite(V5, load);
      Blynk.virtualWrite(V7, SpO2);
      if(digitalRead(15)){      
        Blynk.virtualWrite(V8, "You Have a Call");
      }
      else{
        Blynk.virtualWrite(V8, " ");     
      }
      if(load.toInt() < 10){
        Blynk.virtualWrite(V4, "Saline Finished");
        Blynk.setProperty(V6, "color", "#D3435C");
      }
      else{
        Blynk.virtualWrite(V4, "Saline Ok");
        Blynk.setProperty(V6, "color", "#23C48E");
      }
    }
    else{
      content += c;
    }
  }
}
void sendSensor()
{
   /* t = dht.getTemperature();
    h = dht.getHumidity();
    Serial.print(F(" Temperature: "));
    Serial.print(t);
    Blynk.virtualWrite(V2, String(t) );
    Serial.print(F("°C "));
    sensors.requestTemperatures(); 
    temperatureC = sensors.getTempCByIndex(0);
    Serial.print(temperatureC);
    Serial.println("ºC");
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(0, 0);
    display.println("BPM");
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(30,0);
    display.println(bpm);
  
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(0, 20);
    display.println("Load");  
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(30,20);
    display.println(load);
    
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(0, 40);
    display.println("Temp");  
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(30,40);
    display.println(temperatureC);
    
    display.display();
    Blynk.virtualWrite(V0, temperatureC);
    Blynk.virtualWrite(V1, h);
    Blynk.virtualWrite(V2, t);
    Blynk.virtualWrite(V3, bpm);
    Blynk.virtualWrite(V5, load);
    if(load.toInt() < 10){
      Blynk.virtualWrite(V4, "Saline Finished");
      if (String(BLYNK_RED) != String(gaugeColor)) {
      gaugeColor = BLYNK_RED;
      led1.setColor(BLYNK_RED);
      }
      
    }
    else{
      Blynk.virtualWrite(V4, "Saline Ok");
      if (String(BLYNK_GREEN) != String(gaugeColor)) {
      gaugeColor = BLYNK_GREEN;
     led1.setColor(BLYNK_GREEN);
      }
      
    }*/
}
BLYNK_WRITE(V6) {
  if(load.toInt() < 10){
    if (String(BLYNK_RED) != String(gaugeColor)) {
      gaugeColor = BLYNK_RED;
      led1.setColor(BLYNK_GREEN);
    }
  }
  else{
    if (String(BLYNK_GREEN) != String(gaugeColor)) {
      gaugeColor = BLYNK_GREEN;
      led1.setColor(BLYNK_GREEN);
    }
  }
}
