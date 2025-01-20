#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>

#define sensorPower 7
#define sensorPin A1
#define ONE_WIRE_BUS 12
float calibration_value = 21.34;
int phval = 0; 
unsigned long int avgval; 
int buffer_arr[10],temp;
int val = 0;

SoftwareSerial nodeMcu(3,2); // (Rx, Tx) 

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

 float Celcius=0;
 float Fahrenheit=0;
void setup(void)
{
  pinMode(sensorPower, OUTPUT);
  digitalWrite(sensorPower, LOW);
  Serial.begin(9600);  
  nodeMcu.begin(9600);
  sensors.begin();
}

void loop(void)
{ 
  int level = readSensor();
  sensors.requestTemperatures(); 
  Celcius=sensors.getTempCByIndex(0);
  Serial.print(" C  ");
  Serial.print(Celcius);
  for(int i=0;i<10;i++) 
  { 
    buffer_arr[i]=analogRead(A0);
    delay(30);
  }
  for(int i=0;i<9;i++)
  {
    for(int j=i+1;j<10;j++)
    {
      if(buffer_arr[i]>buffer_arr[j])
      {
        temp=buffer_arr[i];
        buffer_arr[i]=buffer_arr[j];
        buffer_arr[j]=temp;
      }
    }
  }
  avgval=0;
  for(int i=2;i<8;i++)
  avgval+=buffer_arr[i];
  float volt=(float)avgval*5.0/1024/6;
  float ph_act = -3.12 * volt + calibration_value;
  Serial.print(" pH Val:  ");
  Serial.print(ph_act);
  Serial.print(" Water level: ");
  Serial.println(level);
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["Temp"] = Celcius;
  root["PH"] = ph_act;
  root["Level"] = level;
  root.printTo(nodeMcu);
  Serial.println("Json Sent");
  delay(1000);
}


int readSensor() {
  digitalWrite(sensorPower, HIGH);  // Turn the sensor ON
  delay(10);              // wait 10 milliseconds
  val = analogRead(sensorPin);    // Read the analog value form sensor
  digitalWrite(sensorPower, LOW);   // Turn the sensor OFF
  return val;             // send current reading
}
