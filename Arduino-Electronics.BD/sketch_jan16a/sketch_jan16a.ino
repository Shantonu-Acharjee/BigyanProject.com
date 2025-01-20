#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPLpd26x0Zg"
#define BLYNK_DEVICE_NAME "Pulse oximeters"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MLX90614.h>

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
#define REPORTING_PERIOD_MS 1000
PulseOximeter pox;

uint32_t tsLastReport = 0;
#define BLYNK_AUTH_TOKEN "wFOhORUa_35JCXyuMib29cmQ3izzDKFZ"
char auth[] = "wFOhORUa_35JCXyuMib29cmQ3izzDKFZ";
char ssid[] = "mobilewifi";
char pass[] = "abcd1234efgh";
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
  Serial.println("Beat!");
  display.drawBitmap( 60, 20, bitmap, 28, 28, 1);
  display.display();
}
void setup() {
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
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
  display.setTextColor(1);
  display.setCursor(0, 0);

  display.println("Initializing pulse oximeter..");
   display.display();
  Serial.print("Initializing pulse oximeter..");
  if (!mlx.begin()) {
    Serial.println("Error connecting to MLX sensor. Check wiring.");
    while (1);
  };
  delay(1000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(1);
  display.setCursor(0, 0);
  display.println("Connecting to Blynk..");
  display.display();
  Blynk.begin(auth, ssid, pass);
  if (!pox.begin()) {
  Serial.println("FAILED");
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(1);
  display.setCursor(0, 0);
  display.println("FAILED");
  display.display();
  for(;;);
  } else {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(1);
  display.setCursor(0, 0);
  display.println("SUCCESS");
  display.display();
  Serial.println("SUCCESS");
  }
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
  pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop() {
  pox.update();
  Blynk.run();
  float HeartRate = pox.getHeartRate();
  int SpO2 = pox.getSpO2();
  float Ambient = mlx.readAmbientTempC();
  float ObjectTemp = mlx.readObjectTempC();
  if (millis() - tsLastReport > REPORTING_PERIOD_MS)
  {
    Blynk.virtualWrite(V1, HeartRate);
    Blynk.virtualWrite(V2, SpO2);
    Blynk.virtualWrite(V3, Ambient);
    Blynk.virtualWrite(V4, ObjectTemp);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(1);
    display.setCursor(0, 0);
    display.println("BPM");
    display.setTextSize(1);
    display.setTextColor(1);
    display.setCursor(30,0);
    display.println(HeartRate);
  
    display.setTextSize(1);
    display.setTextColor(1);
    display.setCursor(0, 20);
    display.println("Spo2");  
    display.setTextSize(1);
    display.setTextColor(1);
    display.setCursor(30,20);
    display.println(SpO2);
    
    display.setTextSize(1);
    display.setTextColor(1);
    display.setCursor(0, 40);
    display.println("Temp");  
    display.setTextSize(1);
    display.setTextColor(1);
    display.setCursor(30,40);
    display.println(ObjectTemp);
    display.display();
    tsLastReport = millis();
  }
}
