#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN D4
#define RST_PIN D3

// Replace with your network credentials
const char* ssid = "project";
const char* password = "12345678";

MFRC522 mfrc522(SS_PIN, RST_PIN); // Instance of the class

boolean getID();
LiquidCrystal_I2C lcd(0x27,16,2);
// Init array that will store new NUID 
byte readcard[4];
String tagID="";
String User =""; 
int qts=0;
int total=0;
void setup() { 
  Serial.begin(115200);
  pinMode(D8,OUTPUT);
  SPI.begin(); // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Smart Cart");
  delay(2000);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Please Scan");
  lcd.setCursor(0,1);
  lcd.print("Membership Card");
}
 
void loop() {
  if(getID()){
    if(tagID == "233A96C" && User != ""){
      qts++;
      total = total + 50;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Item Qts: ");
      lcd.print(qts);
      lcd.setCursor(0,1);
      lcd.print("Total: ");
      lcd.print(total);
      // wait for WiFi connection
      if ((WiFi.status() == WL_CONNECTED)) {

        std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);

        // Ignore SSL certificate validation
        client->setInsecure();
        
        //create an HTTPClient instance
        HTTPClient https;
        
        //Initializing an HTTPS communication using the secure client
        Serial.print("[HTTPS] begin...\n");
        if (https.begin(*client, "https://cart.shawon.com.bd/insert.php")) {  // HTTPS
          Serial.print("[HTTPS] GET...\n");
          https.addHeader("Content-Type", "application/x-www-form-urlencoded");
          // start connection and send HTTP header
          String httpRequestData = "id=1&user="+User;           
          // Send HTTP POST request
          int httpCode = https.POST(httpRequestData);
          // httpCode will be negative on error
          if (httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
            // file found at server
            if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
              String payload = https.getString();
              Serial.println(payload);
            }
          } else {
            Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
          }

          https.end();
        } else {
          Serial.printf("[HTTPS] Unable to connect\n");
        }
      }
    }
    else if(tagID == "63FC4A17" && User != ""){
      qts++;
      total = total + 60;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Item Qts: ");
      lcd.print(qts);
      lcd.setCursor(0,1);
      lcd.print("Total: ");
      lcd.print(total);
      // wait for WiFi connection
      if ((WiFi.status() == WL_CONNECTED)) {

        std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);

        // Ignore SSL certificate validation
        client->setInsecure();
        
        //create an HTTPClient instance
        HTTPClient https;
        
        //Initializing an HTTPS communication using the secure client
        Serial.print("[HTTPS] begin...\n");
        if (https.begin(*client, "https://cart.shawon.com.bd/insert.php")) {  // HTTPS
          Serial.print("[HTTPS] GET...\n");
          https.addHeader("Content-Type", "application/x-www-form-urlencoded");
          // start connection and send HTTP header
          String httpRequestData = "id=2&user="+User;           
          // Send HTTP POST request
          int httpCode = https.POST(httpRequestData);
          // httpCode will be negative on error
          if (httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
            // file found at server
            if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
              String payload = https.getString();
              Serial.println(payload);
            }
          } else {
            Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
          }

          https.end();
        } else {
          Serial.printf("[HTTPS] Unable to connect\n");
        }
      }
    }
    else if(tagID == "439598A9" && User != ""){
      qts++;
      total = total + 10;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Item Qts: ");
      lcd.print(qts);
      lcd.setCursor(0,1);
      lcd.print("Total: ");
      lcd.print(total);
      // wait for WiFi connection
      if ((WiFi.status() == WL_CONNECTED)) {

        std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);

        // Ignore SSL certificate validation
        client->setInsecure();
        
        //create an HTTPClient instance
        HTTPClient https;
        
        //Initializing an HTTPS communication using the secure client
        Serial.print("[HTTPS] begin...\n");
        if (https.begin(*client, "https://cart.shawon.com.bd/insert.php")) {  // HTTPS
          Serial.print("[HTTPS] GET...\n");
          https.addHeader("Content-Type", "application/x-www-form-urlencoded");
          // start connection and send HTTP header
          String httpRequestData = "id=3&user="+User;            
          // Send HTTP POST request
          int httpCode = https.POST(httpRequestData);
          // httpCode will be negative on error
          if (httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
            // file found at server
            if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
              String payload = https.getString();
              Serial.println(payload);
            }
          } else {
            Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
          }

          https.end();
        } else {
          Serial.printf("[HTTPS] Unable to connect\n");
        }
      }
    }
    else if(tagID == "7396BE1A" && User == ""){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Please Wait...");
      // wait for WiFi connection
      if ((WiFi.status() == WL_CONNECTED)) {

        std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);

        // Ignore SSL certificate validation
        client->setInsecure();
        
        //create an HTTPClient instance
        HTTPClient https;
        
        //Initializing an HTTPS communication using the secure client
        Serial.print("[HTTPS] begin...\n");
        if (https.begin(*client, "https://cart.shawon.com.bd/get_user_info.php")) {  // HTTPS
          Serial.print("[HTTPS] GET...\n");
          https.addHeader("Content-Type", "application/x-www-form-urlencoded");
          // start connection and send HTTP header
          String httpRequestData = "id=1";           
          // Send HTTP POST request
          int httpCode = https.POST(httpRequestData);
          // httpCode will be negative on error
          if (httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
            // file found at server
            if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
              User = https.getString();
              Serial.println(User);
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("Welcome");
              lcd.setCursor(0,1);
              lcd.print(User);
            }
          } else {
            Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
          }

          https.end();
        } else {
          Serial.printf("[HTTPS] Unable to connect\n");
        }
      }
    }
    else if(tagID == "13BD741B" && User == ""){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Please Wait...");
      // wait for WiFi connection
      if ((WiFi.status() == WL_CONNECTED)) {

        std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);

        // Ignore SSL certificate validation
        client->setInsecure();
        
        //create an HTTPClient instance
        HTTPClient https;
        
        //Initializing an HTTPS communication using the secure client
        Serial.print("[HTTPS] begin...\n");
        if (https.begin(*client, "https://cart.shawon.com.bd/get_user_info.php")) {  // HTTPS
          Serial.print("[HTTPS] GET...\n");
          https.addHeader("Content-Type", "application/x-www-form-urlencoded");
          // start connection and send HTTP header
          String httpRequestData = "id=2";           
          // Send HTTP POST request
          int httpCode = https.POST(httpRequestData);
          // httpCode will be negative on error
          if (httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
            // file found at server
            if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
              User = https.getString();
              Serial.println(User);
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("Welcome");
              lcd.setCursor(0,1);
              lcd.print(User);
            }
          } else {
            Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
          }

          https.end();
        } else {
          Serial.printf("[HTTPS] Unable to connect\n");
        }
      }
    }
    else{
      //lcd.clear();
      //lcd.setCursor(0,0);
      //lcd.print(tagID);
    }
  }
}


/**
 * Helper routine to dump a byte array as hex values to Serial. 
 */
boolean getID() {
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! mfrc522.PICC_IsNewCardPresent())
    return false;

  // Verify if the NUID has been readed
  if ( ! mfrc522.PICC_ReadCardSerial())
    return false;

  tagID="";
  for(uint8_t i=0; i<4; i++){
    tagID.concat(String(mfrc522.uid.uidByte[i],HEX));
  }
  tagID.toUpperCase();
  mfrc522.PICC_HaltA();
  return true;
}

