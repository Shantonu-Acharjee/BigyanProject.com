#include <HX711_ADC.h>
#if defined(ESP8266)|| defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif
#include "U8glib.h"  // Include U8glib library
// Create display and set pins:
U8GLIB_PCD8544 u8g(52, 51, 45, 47, 49);   // (CLK=13, DIN=11, CE=10, DC=9, RST=8)
int product_count = 0;
int cart_row = 0;
//pins:
const int HX711_dout = 4; //mcu > HX711 dout pin
const int HX711_sck = 5; //mcu > HX711 sck pin

//HX711 constructor:
HX711_ADC LoadCell(HX711_dout, HX711_sck);

const int calVal_eepromAdress = 0;
unsigned long t = 0;
const char* product_names[][4] =         {
            { "", "", ""},
            { "ITEM 1", "1", "100"},
            { "ITEM 2", "2", "200" },
            { "ITEM 3", "3", "300"},
        };
int cart[3];
int total;
String content = "";
void setup(void)  // Start of setup
{
  Serial.begin(115200);
  Serial1.begin(115200);
  pinMode(42, OUTPUT);
  pinMode(43, OUTPUT);
  u8g.setFont(u8g_font_6x10);
  u8g.setRot180();
  clearOLED();
  LoadCell.begin();
  //LoadCell.setReverseOutput(); //uncomment to turn a negative output value to positive
  float calibrationValue; // calibration value (see example file "Calibration.ino")
  calibrationValue = 696.0; // uncomment this if you want to set the calibration value in the sketch
#if defined(ESP8266)|| defined(ESP32)
  //EEPROM.begin(512); // uncomment this if you use ESP8266/ESP32 and want to fetch the calibration value from eeprom
#endif
  EEPROM.get(calVal_eepromAdress, calibrationValue); // uncomment this if you want to fetch the calibration value from eeprom

  unsigned long stabilizingtime = 2000; // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
  boolean _tare = true; //set this to false if you don't want tare to be performed in the next step
  LoadCell.start(stabilizingtime, _tare);
  if (LoadCell.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
  }
  else {
    LoadCell.setCalFactor(calibrationValue); // set calibration value (float)
    Serial.println("Startup is complete");
  }
}  // End of setup
void loop(void)  // Start of loop
{  
  if (Serial1.available())  {
    char c = Serial1.read();  //gets one byte from serial buffer
    if (c == '\n') {
      Serial.println(content);
      content.trim();
        int productID = content.toInt();        
        content = "";
        Serial1.flush();
        u8g.firstPage(); // Beginning of the picture loop
        do  // Include all you want to show on the display:
        {
          u8g.setFont(u8g_font_5x7);
          u8g.drawStr(0, 7, "  SHOPPING CART");
          u8g.setFont(u8g_font_4x6);
          u8g.drawStr(0, 14, "ITEM");
          u8g.drawStr(40, 14, "QTY");
          u8g.drawStr(60, 14, "PRICE");
          u8g.drawLine(0, 15, 83, 15);
          u8g.drawLine(37, 8, 37, 40);
          u8g.drawLine(57, 8, 57, 40);
          u8g.drawLine(0, 40, 83, 40);
          u8g.drawStr(35, 48, "TOTAL=");
          u8g.setPrintPos(60, 48);  // (x,y)
          u8g.print(total);
          
          for(size_t i=0; i<sizeof(cart) / sizeof(cart[0]); i++){
            if(cart[i] == NULL){
              break; 
            }
            if(i==0){
              u8g.drawStr(0, 23, product_names[cart[i]][0]); 
              u8g.drawStr(40, 23, product_names[cart[i]][1]); 
              u8g.drawStr(60, 23, product_names[cart[i]][2]); 
            }
            if(i==1){
              u8g.drawStr(0, 31, product_names[cart[i]][0]); 
              u8g.drawStr(40, 31, product_names[cart[i]][1]); 
              u8g.drawStr(60, 31, product_names[cart[i]][2]); 
            }
            if(i==2){
              u8g.drawStr(0, 39, product_names[cart[i]][0]); 
              u8g.drawStr(40, 39, product_names[cart[i]][1]); 
              u8g.drawStr(60, 39, product_names[cart[i]][2]); 
            }
            Serial.println(product_names[cart[i]][2]);
          }   
        } while(u8g.nextPage());  // End of the picture loop 
        if(product_count == 0){
          cart[0] = productID;  
          int val1 = atoi(product_names[productID][2]);
          total = total + val1;
        }
        if(product_count == 1){
          cart[1] = productID;  
          int val1 = atoi(product_names[productID][2]);
          total = total + val1;
        }
        if(product_count == 2){
          cart[2] = productID;   
          int val1 = atoi(product_names[productID][2]);
          total = total + val1;
        }
      
        
        product_count++;
        if(product_count > 4){
          product_count = 0;
          clearOLED();
          cart[0] = 0;
          cart[1] = 0;
          cart[2] = 0;
          total = 0;
        }
      
    }
    else{
      content += c;
    }
  }
  static boolean newDataReady = 0;
  const int serialPrintInterval = 0; //increase value to slow down serial print activity

  // check for new data/start next conversion:
  if (LoadCell.update()) newDataReady = true;

  // get smoothed value from the dataset:
  if (newDataReady) {
    if (millis() > t + serialPrintInterval) {
      float i = LoadCell.getData();
      Serial.print("Load_cell output val: ");
      Serial.println(i);
      Serial.println(product_count);
      if(product_count == 0 && i > 90){
      Serial.println("0");
        digitalWrite(42, HIGH);
        digitalWrite(43, LOW);
      }
      else if(product_count == 1 && i > 90){
      Serial.println("1");
        digitalWrite(42, HIGH);
        digitalWrite(43, LOW);
      }
      else if(product_count == 2 && i > 190){
      Serial.println("2");
        digitalWrite(42, HIGH);
        digitalWrite(43, LOW);
      }
      else if(product_count == 3 && i > 290){
      Serial.println("3");
        digitalWrite(42, HIGH);
        digitalWrite(43, LOW);
      }
      else if(product_count == 4 && i > 390){
      Serial.println("4");
        digitalWrite(42, HIGH);
        digitalWrite(43, LOW);
      }
      else{
      Serial.println("5");
        digitalWrite(42, LOW);
        digitalWrite(43, HIGH);
      }
      newDataReady = 0;
      t = millis();
    }
  }
}  // End of loop
void clearOLED(){
  u8g.firstPage();
  do {
    u8g.setFont(u8g_font_5x7);
    u8g.drawStr(0, 7, "  SHOPPING CART");
    u8g.setFont(u8g_font_4x6);
    u8g.drawStr(0, 14, "ITEM");
    u8g.drawStr(40, 14, "QTY");
    u8g.drawStr(60, 14, "PRICE");
    u8g.drawLine(0, 15, 83, 15);
    u8g.drawLine(37, 8, 37, 40);
    u8g.drawLine(57, 8, 57, 40);
    u8g.drawLine(0, 40, 83, 40);
    u8g.drawStr(35, 48, "TOTAL=");
  } while( u8g.nextPage() );
}
