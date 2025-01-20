#include <LiquidCrystal.h>
#include <hidboot.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
class KbdRptParser : public KeyboardReportParser
{
 
protected:
  virtual void OnKeyDown  (uint8_t mod, uint8_t key);
  virtual void OnKeyPressed(uint8_t key);
};
 
void KbdRptParser::OnKeyDown(uint8_t mod, uint8_t key)  
{
    uint8_t c = OemToAscii(mod, key);
 
    if (c)
        OnKeyPressed(c);
}
 
/* what to do when symbol arrives */
void KbdRptParser::OnKeyPressed(uint8_t key)  
{
static uint32_t next_time = 0;      //watchdog
static uint8_t current_cursor = 0;  //tracks current cursor position  
 
    if( millis() > next_time ) {
      lcd.clear();
      current_cursor = 0;
      delay( 5 );  //LCD-specific 
      lcd.setCursor( 0,0 );
    }//if( millis() > next_time ...
 
    next_time = millis() + 200;  //reset watchdog
 
    if( current_cursor++ == ( DISPLAY_WIDTH + 1 )) {  //switch to second line if cursor outside the screen
      lcd.setCursor( 0,1 );
    }
 
    Serial.println( key );
    lcd.print( key );
};
 
KbdRptParser Prs;
 
void setup()
{
    Serial.begin( 115200 );
    Serial.println("Start");
 
    if (Usb.Init() == -1) {
        Serial.println("OSC did not start.");
    }
 
    delay( 200 );
 
    Keyboard.SetReportParser(0, (HIDReportParser*)&Prs);
    // set up the LCD's number of columns and rows: 
    lcd.begin(16, 2);
    lcd.clear();
    lcd.noAutoscroll();
    lcd.print("Ready");
    delay( 200 );
}
 
void loop()
{
  Usb.Task();
}
