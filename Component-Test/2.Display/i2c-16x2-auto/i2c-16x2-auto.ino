/*
Analog Pin 4 - SDA
Analog pin 5 - SCL
5V - Vcc
GND - GND
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // Temporary address; will be updated
byte lcdAddress = 0x00;  // Variable to store detected address

void setup() {
  Wire.begin();
  Serial.begin(9600);
  while (!Serial);
  Serial.println("\nI2C Scanner and Auto LCD Display");

  lcdAddress = scanI2CAddress();  // Scan for I2C device

  if (lcdAddress != 0x00) {
    Serial.print("LCD Found at 0x");
    if (lcdAddress < 16) Serial.print("0");
    Serial.println(lcdAddress, HEX);

    lcd = LiquidCrystal_I2C(lcdAddress, 16, 2);  // Re-initialize with detected address
    lcd.init();
    lcd.backlight();
    lcd.setCursor(1, 0);
    lcd.print("Bigyan Project");
    lcd.setCursor(4, 1);
    lcd.print("Shantonu");
  } else {
    Serial.println("No I2C LCD device found.");
  }
}

void loop() {
  // Nothing needed here; you can add more actions if needed
}

byte scanI2CAddress() {
  byte error, address;
  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      return address;  // Return the first detected device address
    }
  }
  return 0x00; // Return zero if no device found
}
