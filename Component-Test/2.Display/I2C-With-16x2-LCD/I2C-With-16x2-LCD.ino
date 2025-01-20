#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 (default for many I2C LCDs)
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // Initialize the LCD
  lcd.init();
  lcd.backlight(); // Turn on the backlight
  lcd.setCursor(0, 0); // Set cursor to the first row, first column
  lcd.print("Hello, World!"); // Print a message on the first row
  lcd.setCursor(0, 1); // Set cursor to the second row
  lcd.print("LCD Test OK"); // Print a message on the second row
}

void loop() {
  // You can add other test functionality here if needed
}
