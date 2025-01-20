// for Arduino Microcontrollers
//int latchPin_PISO = 3;
//int clockPin_PISO = 4;
//int dataPin_PISO = 2;

// for ESP8266 Microcontrollers
//int latchPin_PISO = D6;
//int clockPin_PISO = D5;
//int dataPin_PISO = D7;

// for ESP32 Microcontrollers
int latchPin = 18;
int clockPin = 19;
int dataPin = 5;

int j;
int value;
byte switchVar = 0; // for pull down configuration
//byte switchVar = 1; // for pull up configuration

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, INPUT);  
  pinMode(clockPin, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  byte dataIn = 0;
  digitalWrite(latchPin, 0);
  digitalWrite(clockPin, 0);
  digitalWrite(clockPin, 1);

  digitalWrite(latchPin, 1);
  
  for(j = 0; j < 8; j++) {
    value = digitalRead(dataPin);
    Serial.print("Position of Bit in 8-bit array: ");
    Serial.println(j);
    Serial.print("Value of Bit in 8-bit array: ");
    Serial.println(value);
    if (value) {
      int a = (1 << j);
      dataIn = dataIn | a;
      }
      digitalWrite(clockPin, LOW);
      digitalWrite(clockPin, HIGH);
    }

    if (switchVar != dataIn)
    {
      switchVar = dataIn;
      Serial.print("dataIn DEC: ");
      Serial.print(dataIn, DEC);
      Serial.println();
      Serial.print("dataIn BIN: ");
      Serial.print(dataIn, BIN);
      Serial.println();
    }
  delay(5000);
} 
