#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPLEwhX7xCO"
#define BLYNK_DEVICE_NAME "IOT Project"
#define BLYNK_AUTH_TOKEN "v3rFY8D50xuYM3fIRwJ4PzKM2lvOJRFe"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Servo.h>
Servo myservo;
#define ONE_WIRE_BUS 13
#define SOUND_VELOCITY 0.034
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
BlynkTimer timer;
char auth[] = "v3rFY8D50xuYM3fIRwJ4PzKM2lvOJRFe";
const int trigPin = 12;
const int echoPin = 14;

char ssid[] = "mobilewifi";
char pass[] = "abcd1234efgh";

long duration;
float distanceCm;
BLYNK_WRITE(V0)
{
  if(param.asInt()==1){
    digitalWrite(4,HIGH);
    digitalWrite(5,LOW);
  }
  else{
    digitalWrite(4,LOW);
    digitalWrite(5,LOW);
  }
}
BLYNK_WRITE(V1)
{
  if(param.asInt()==1){
    myservo.write(45);
  }
  else{
    myservo.write(90);
  }
}
BLYNK_WRITE(V2)
{
  if(param.asInt()==1){
    myservo.write(135);
  }
  else{
    myservo.write(90);
  }
}
BLYNK_WRITE(V3)
{
  if(param.asInt()==1){
    digitalWrite(15,HIGH);
  }
  else{
    digitalWrite(15,LOW);
  }
}

BLYNK_CONNECTED()
{
  Blynk.syncVirtual(V0);  

  Blynk.syncVirtual(V1);  

  Blynk.syncVirtual(V2);   
}

void setup()
{
  Serial.begin(115200);
  myservo.attach(0, 500, 2400);
  pinMode(13, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(15, OUTPUT); 
  digitalWrite(15,HIGH);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  sensors.begin();
  myservo.write(90);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  Blynk.run();
  timer.run();      
}
void sendSensor()
{
  int gas = analogRead(A0);
  Serial.println(gas);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distanceCm = duration * SOUND_VELOCITY/2;
  
  
  // Prints the distance on the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  Serial.print(temperatureC);
  Serial.println("ºC");
        
  Blynk.virtualWrite(V4, distanceCm);
  Blynk.virtualWrite(V5, temperatureC);
  Blynk.virtualWrite(V6, gas);
}
