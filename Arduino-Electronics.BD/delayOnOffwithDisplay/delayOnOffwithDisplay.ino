#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2
double count=0;



#define LOGO16_GLCD_HEIGHT 16 // do not change this. Error in video
#define LOGO16_GLCD_WIDTH  16 // do not change this. Error in video

    // look at line 27 to 30 of Adafruit_SSD1306.h inside the library to select the dimensions
#if (SSD1306_LCDHEIGHT != 32) // 
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
int onPin = A0;
int offPin = A1;
int relayPin = 12;
int flag = 0;
int onArray[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int offArray[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
unsigned long lastDebounceTime = 0;
unsigned long lastDebounceTime1 = 0;
unsigned long offValue = 0;
unsigned long onValue = 0;
unsigned long OldoffValue = 0;
unsigned long OldonValue = 0;

float onValue1 = 0.0;
String vString = " ";
String vString1 = " ";

void setup() {
  //Serial.begin(9600);
  delay(3000);
  pinMode(relayPin, INPUT_PULLUP);
  
  lastDebounceTime = millis();
  lastDebounceTime1 = millis();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  delay(1000);
  display.clearDisplay();
  pinMode(relayPin, OUTPUT);
}

void loop() {
  
  for (int i = 0; i < 10; i++) {
    onValue = analogRead(onPin);
    offValue = analogRead(offPin);
    //Serial.print(onValue);
    onArray[i] = onValue;
    offArray[i] = offValue;
    delay(5);
  }
  int n = 10;
  onValue = mostFrequent(onArray, n);
  offValue = mostFrequent(offArray, n);
  //Serial.println();
  //Serial.println(onValue);
  //Serial.println();
  onValue = map(onValue, 0, 1023, 0, 3600)*1000;
  offValue = map(offValue, 0, 1023, 0, 3600)*1000;
  onValue1 = (float)onValue/1000.0;
  if(onValue1>1){
    vString =  String(onValue1)+" Second";// using a float and the 
    onValue1 = (float)onValue/60000.0;
    if(onValue1>=1){
     vString =  String(onValue1)+" Minute";// using a float and the 
    }
  }
  else{
    vString =  String(onValue)+" Milisecond";
  }
  onValue1 = (float)offValue/1000.0;
  if(onValue1>1){
    vString1 =  String(onValue1)+" Second";// using a float and the 
    onValue1 = (float)offValue/60000.0;
    if(onValue1>=1){
     vString1 =  String(onValue1)+" Minute";// using a float and the 
    }
  }
  else{
    vString1 =  String(offValue)+" Milisecond";
  }
  if((offValue != OldoffValue) || (onValue != OldonValue))
  {
    display.clearDisplay();
    robojaxText("On: "+vString, 10, 7, 1, false);
    robojaxText("Off: "+vString1, 10, 20, 1, false);
    display.drawRect(1, 1, 127,31, WHITE);
    display.display();
  }
  if (!digitalRead(relayPin) && (millis() - lastDebounceTime) > offValue) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:
    digitalWrite(relayPin, HIGH);
    lastDebounceTime = millis();
    lastDebounceTime1 = millis();
  }
  if (digitalRead(relayPin) && (millis() - lastDebounceTime1) > onValue) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:
    digitalWrite(relayPin, LOW);
    lastDebounceTime = millis();
    lastDebounceTime1 = millis();
  }
  delay(50);
  OldonValue = onValue;
  OldoffValue = offValue;
}
void robojaxText(String text, int x, int y,int size, boolean d) {

  display.setTextSize(size);
  display.setTextColor(WHITE);
  display.setCursor(x,y);
  display.println(text);
  if(d){
    display.display();
  }
}
int mostFrequent(int a[], int n) 
{ 
    int count = 1, tempCount;
  int popular = a[0];
  int temp = 0;
  for (int i = 0; i < (n - 1); i++)
  {
    temp = a[i];
    tempCount = 0;
    for (int j = 1; j < n; j++)
    {
      if (temp == a[j])
        tempCount++;
    }
    if (tempCount > count)
    {
      popular = temp;
      count = tempCount;
    }
  }
  return popular; 
} 
