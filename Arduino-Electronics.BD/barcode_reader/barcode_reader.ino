#include <usbhid.h>
#include <hiduniversal.h>
#include <avr/pgmspace.h>
 
#include <Usb.h>
#include <usbhub.h>
#include <avr/pgmspace.h>
#include <hidboot.h>

 
USB     Usb;
USBHub     Hub(&Usb);
HIDUniversal    Hid(&Usb);

String code;
 
class KbdRptParser : public KeyboardReportParser
{
 void PrintKey(uint8_t mod, uint8_t key);
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
 code.concat(key);
 if(code == "78643335383541944133419413")
 {
  Serial.println(code);
  code = "";
 }
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
 
    Hid.SetReportParser(0, (HIDReportParser*)&Prs);
    delay( 200 );
}
 
void loop()
{
  Usb.Task();
}
