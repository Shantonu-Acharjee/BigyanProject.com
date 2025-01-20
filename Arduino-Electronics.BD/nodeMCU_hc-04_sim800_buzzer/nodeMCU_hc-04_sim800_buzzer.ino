#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
SoftwareSerial mySerial(5, 4); // TX, RX

const int trigPin = 12;
const int echoPin = 14;
const int buzzerPin = 16;
String apiKey = "UXNL7QY7KENA64C0"; // Enter your Write API key from ThingSpeak
const char *ssid = "Project";     // replace with your wifi ssid and wpa2 key
const char *pass = "abcd1234efgh";
const char *server = "api.thingspeak.com";
//define sound velocity in cm/uS
#define SOUND_VELOCITY 0.034
#define rainPin A0

long duration;
float distanceCm;
const long interval = 5000;
unsigned long previousMillis = 0;
WiFiClient client;

void setup() {
  Serial.begin(115200); // Starts the serial communication
  mySerial.begin(9600);
  delay(15000);
  pinMode(buzzerPin, OUTPUT); // Sets the buzzerPin as an Output
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  digitalWrite(buzzerPin,HIGH);
  WiFi.begin(ssid, pass);
  
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  mySerial.println("AT"); //If everything is Okay it will show "OK" on the serial monitor
  updateSerial();
  mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  mySerial.println("AT+CNMI=2,2,0,0,0"); //Configure the SIM800L on how to manage the Received SMS... Check the SIM800L AT commands manual
  updateSerial();
}

void loop() {
  unsigned long currentMillis = millis();
  // Clears the trigPin
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
  
  //distanceCm = map(distanceCm, 0, 200, 0, 100);
  // Prints the distance on the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.print(distanceCm);
  int rainvalue = analogRead (rainPin);
  Serial.print(" Rain: ");
  Serial.println(rainvalue);
  if(distanceCm <= 10){
    digitalWrite(buzzerPin,LOW);
    send_sms();
    delay(10000);
  }
  else{
    digitalWrite(buzzerPin,HIGH);

  }
  if (currentMillis - previousMillis >= interval) {
    Serial.println("IN");
    if (client.connect(server, 80)) // "184.106.153.149" or api.thingspeak.com
    {
      String postStr = apiKey;
        postStr += "&field1=";
        postStr += String(distanceCm);
        postStr += "&field2=";
        postStr += String(rainvalue);
        postStr += "\r\n\r\n";
      
      client.print("POST /update HTTP/1.1\n");
      client.print("Host: api.thingspeak.com\n");
      client.print("Connection: close\n");
      client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
      client.print("Content-Type: application/x-www-form-urlencoded\n");
      client.print("Content-Length: ");
      client.print(postStr.length());
      client.print("\n\n");
      client.print(postStr);
    
    }
    client.stop();
    previousMillis = millis();
  }
  delay(1000);
}
void send_sms()
{  
  mySerial.println("AT");
  updateSerial();

  mySerial.println("AT+CMGF=1");
  updateSerial();
  mySerial.println("AT+CMGS=\"+8801552443129\"");
  updateSerial();
  mySerial.print("Water Full");
  updateSerial();
  mySerial.write(26);
}
void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());
  }
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());
  }
}