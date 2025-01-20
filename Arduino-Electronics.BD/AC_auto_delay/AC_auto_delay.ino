#include <EEPROM.h>
#include <LiquidCrystal.h>
#include <Wire.h>
int addr = 0;
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

byte custchar[8][8] = {
    {
        B11111,
        B11111,
        B11111,
        B00000,
        B00000,
        B00000,
        B00000,
        B00000
    }, {
        B00000,
        B00000,
        B00000,
        B00000,
        B00000,
        B11111,
        B11111,
        B11111
    }, {
        B11111,
        B11111,
        B11111,
        B00000,
        B00000,
        B11111,
        B11111,
        B11111
    }, {
        B00000,
        B00000,
        B00000,
        B00000,
        B00000,
        B01110,
        B01110,
        B01110
    }, {
        B00000,
        B00000,
        B00000,
        B01110,
        B01110,
        B01110,
        B00000,
        B00000
    }, {
        B00000,
        B00000,
        B00000,
        B00000,
        B00000,
        B00000,
        B00000,
        B00000
    }, {
        B00000,
        B00000,
        B00000,
        B00000,
        B00000,
        B00000,
        B00000,
        B00000
    }, {
        B00000,
        B00000,
        B00000,
        B00000,
        B00000,
        B00000,
        B00000,
        B00000
    }
};

byte bignums[10][2][3] = {
    {
        {255, 0, 255},
        {255, 1, 255}
    }, {
        {0, 255, 254},
        {1, 255, 1}
    }, {
        {2, 2, 255},
        {255, 1, 1}
    }, {
        {0, 2, 255},
        {1, 1, 255}
    }, {
        {255, 1, 255},
        {254, 254, 255}
    }, {
        {255, 2, 2},
        {1, 1, 255}
    }, {
        {255, 2, 2},
        {255, 1, 255}
    }, {
        {0, 0, 255},
        {254, 255, 254}
    }, {
        {255, 2, 255},
        {255, 1, 255}
    }, {
        {255, 2, 255},
        {254, 254, 255}
    }
};

void loadchars() {
    for (int i = 0; i < 8; i++)
        lcd.createChar(i, custchar[i]);
    
/*    
    lcd.command(64);
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            lcd.write(custchar[j]);
*/
    lcd.home();
}

void printbigchar(byte digit, byte col, byte row, byte symbol = 0) {
    if (digit > 9) return;
    for (int i = 0; i < 2; i++) {
        lcd.setCursor(col, row + i);
        for (int j = 0; j < 3; j++) {
            lcd.write(bignums[digit][i][j]);     //.kbv add the [i]
        }
        lcd.write(254);
    }
    if (symbol == 1) {
        lcd.setCursor(col + 3, row + 1);
        lcd.write(3);
    } else if (symbol == 2) {
        lcd.setCursor(col + 3, row);
        lcd.write(4);
        lcd.setCursor(col + 3, row + 1);
        lcd.write(4);
    }

    lcd.setCursor(col + 4, row);
}

void setup() {
    lcd.begin(20, 4);           //.kbv add the begin()
    pinMode(13, OUTPUT);
    loadchars();
    digitalWrite(13, 1);

    printbigchar(0, 0, 0);
    printbigchar(1, 4, 0);
    printbigchar(2, 8, 0);
    printbigchar(3, 12, 0);
    printbigchar(4, 16, 0, 1);
    printbigchar(5, 0, 2);
    printbigchar(6, 4, 2);
    printbigchar(7, 8, 2);
    printbigchar(8, 12, 2);
    printbigchar(9, 16, 2, 2);
}

void loop() {}
/*
void setup() {
  lcd.begin(20, 4);
  pinMode(8, OUTPUT);
  pinMode(12, OUTPUT);
}


void loop() {
  lcd.setCursor(0, 1);
  if(EEPROM.read(addr) == 2)
  {
    EEPROM.write(addr, 1);
    digitalWrite(8, HIGH);
    digitalWrite(12, LOW);  
    delay(7200000); 
  }
  else
  {     
    EEPROM.write(addr, 2);           
    digitalWrite(8, LOW);    
    digitalWrite(12, HIGH);    
    delay(7200000);              
  }
}*/
