#include <SoftwareSerial.h>
SoftwareSerial mySerial(8, 7); // RX, TX

void setup()
{
  Serial.begin(115200);
  mySerial.begin(115200);
  Serial.println("Initializing..."); 
  delay(1000);
  //delay(15000);
  Serial.println("done...");
  mySerial.println("AT"); //If everything is Okay it will show "OK" on the serial monitor
  updateSerial();
  mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  mySerial.println("AT+CNMI=2,2,0,0,0"); //Configure the SIM800L on how to manage the Received SMS... Check the SIM800L AT commands manual
  updateSerial();
  send_sms();
}

void loop()
{
  updateSerial();
  //delay(5000);
  //send_sms();  
}

void send_sms()
{  
  mySerial.println("AT");
  updateSerial();

  mySerial.println("AT+CMGF=1");
  updateSerial();
  mySerial.println("AT+CMGS=\"+8801983715331\"");
  updateSerial();
  mySerial.print("Fire detected...! take action immediately.");
  updateSerial();
  mySerial.write(26);
}
void gprs(){
  mySerial.println("AT+CGATT?");
  updateSerial();
  mySerial.println("AT+CSTT=\"gpinternet\"");
  updateSerial();
  mySerial.println("AT+CIICR");
  updateSerial();
  mySerial.println("AT+CIFSR");
  updateSerial();
  mySerial.println("AT+CIPSPRT=0");
  updateSerial();
  mySerial.println("AT+CIPSTART=\"tcp\",\"27.131.12.43\",\"1880\"");
  updateSerial();
  mySerial.println("AT+CIPSEND");
  updateSerial();
  mySerial.print("POST ");
  updateSerial();
  mySerial.print("//api/Telemetry");
  updateSerial();
  mySerial.print(" HTTP/1.1\r\nHost: ");
  updateSerial();
  mySerial.print("27.131.12.43");
  updateSerial();
  mySerial.print("\r\n");
  updateSerial();
  mySerial.print("User-Agent: Arduino\r\n");
  updateSerial();
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
void ReceiveMode(){       //Set the SIM800L Receive mode
  
  mySerial.println("AT"); //If everything is Okay it will show "OK" on the serial monitor
  updateSerial();
  mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  mySerial.println("AT+CNMI=2,2,0,0,0"); //Configure the SIM800L on how to manage the Received SMS... Check the SIM800L AT commands manual
  updateSerial();
}
