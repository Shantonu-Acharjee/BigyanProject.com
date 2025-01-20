// Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 3

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
const int fan_control_pin = 9;
int count = 0;
unsigned long start_time;
int rpm;

void setup() {
  pinMode(fan_control_pin, OUTPUT);
  analogWrite(fan_control_pin,0);
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(2), counter, RISING);
  // Start up the library
  sensors.begin();

}

void loop() {
  sensors.requestTemperatures();
  int tempC = sensors.getTempCByIndex(0);
  Serial.println(tempC);
  int pwm = 0;
  if(tempC > 40){
    pwm = 255;
  }
  else if(tempC > 35){
    pwm = 200;
  }
  else if(tempC > 30){
    pwm = 100;
  }
  else{
    pwm = 0;
  }
    analogWrite(fan_control_pin, pwm);
    delay(5000);
    start_time = millis();
    count = 0;
    while((millis() - start_time) <1000){
    }
    rpm = count * 30;
    Serial.print("PWM = ");
    Serial.print(map(pwm, 0, 255, 0, 100));
    Serial.print("% , Speed = ");
    Serial.print(rpm);
    Serial.println(" rpm");
  }
void counter(){
  count++;
}
