#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <FirebaseESP8266.h>
#include <SPI.h>
#include <MFRC522.h>

constexpr uint8_t RST_PIN = D3;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = D4;     // Configurable, see typical pin layout above
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;
String tag;
SoftwareSerial uno(D1,D2); // (Rx, Tx)

#define WIFI_SSID "Sarah"
#define WIFI_PASSWORD "abcd1234efgh"

#define FIREBASE_HOST "https://officers-smart-notecase-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "556ALdsQLNKJBOAJkRqctPUX6DNSEODdm9VaMpqE"

String path = "/session/";
//Define FirebaseESP8266 data object
FirebaseData fbdo;

void printResult(FirebaseData &data);

void setup()
{
  Serial.begin(9600);
  uno.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522
  
  pinMode(D1,INPUT);
  pinMode(D2,OUTPUT);
  delay(2000);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["Data1"] = "Connecting Wi-Fi";
  root["Data2"] = "";
  root.printTo(uno);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  root["Data1"] = "Wi-Fi Connected";
  root["Data2"] = "";
  root.printTo(uno);

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  //Set the size of WiFi rx/tx buffers in the case where we want to work with large data.
  fbdo.setBSSLBufferSize(1024, 1024);

  //Set the size of HTTP response buffers in the case where we want to work with large data.
  fbdo.setResponseSize(1024);

  //Set database read timeout to 1 minute (max 15 minutes)
  Firebase.setReadTimeout(fbdo, 1000 * 60);
  //tiny, small, medium, large and unlimited.
  //Size and its write timeout e.g. tiny (1s), small (10s), medium (30s) and large (60s).
  Firebase.setwriteSizeLimit(fbdo, "tiny");
  delay(1000);
  root["Data1"] = " Officers Smart ";
  root["Data2"] = "    Notecase    ";
  root.printTo(uno);

  
}

void loop()
{  
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  if ( ! rfid.PICC_IsNewCardPresent())
    return;
  if (rfid.PICC_ReadCardSerial()) {
    for (byte i = 0; i < 4; i++) {
      tag += rfid.uid.uidByte[i];
    }
    Serial.print(tag);
    Firebase.getInt(fbdo, path + tag);
    if(fbdo.intData() == 400)
    {
      Firebase.setInt(fbdo, path + tag, 0);
      Serial.print("yes");      
      root["Data1"] = "RDID: "+String(tag);
      root["Data2"] = "Access Granted";
      root.printTo(uno);
    }
    else
    {
      Firebase.setInt(fbdo, path + tag, 1);
      Serial.print("no");
      root["Data1"] = "RDID: "+String(tag);
      root["Data2"] = "Access Denied";
      root.printTo(uno);
    }
    Serial.println("PATH: " + fbdo.dataPath());
    Serial.println("TYPE: " + fbdo.dataType());
    Serial.println("ETag: " + fbdo.ETag());
    Serial.println(fbdo.intData());
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
    tag = "";
  }
  delay(5000);
  root["Data1"] = " Officers Smart ";
  root["Data2"] = "    Notecase    ";
  root.printTo(uno);
}
