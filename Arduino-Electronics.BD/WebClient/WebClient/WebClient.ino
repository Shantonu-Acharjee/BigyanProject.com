#include <SPI.h>
#include <Ethernet.h>
#include <Wire.h>
char message[100];
char str;
// assign a MAC address for the ethernet controller.
// fill in your address here:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
// fill in an available IP address on your network here,
// for manual configuration:
IPAddress ip(192, 168, 1, 100);

// fill in your Domain Name Server address here:
IPAddress myDns(1, 1, 1, 1);

// initialize the library instance:
EthernetClient client;

IPAddress server(192,168,1,104);

unsigned long lastConnectionTime = 0;             // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 10L * 1000L; // delay between updates, in milliseconds
// the "L" is needed to use long type numbers

//******************************************************* MAIN PROGRAM START ************************************************
void setup() {
  // start serial port:
  Serial.begin(115200);
  Wire.begin();
 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Waiting....");
   delay(5000);
   Serial.println("Start");
  // give the ethernet module time to boot up:
  // start the Ethernet connection using a fixed IP address and DNS server:
  Ethernet.begin(mac, ip, myDns);
  // print the Ethernet board/shield's IP address:
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());
}

void loop() {
  // if there's incoming data from the net connection.
  // send it out the serial port.  This is for debugging
  // purposes only:
  if (client.available()) {
    char c = client.read();
    Serial.write(c);
  }
  // if ten seconds have passed since your last connection,
  // then connect again and send data:
  if (millis() - lastConnectionTime > postingInterval) {
    httpRequest();
  }

}

// this method makes a HTTP connection to the server:
void httpRequest() {
  // close any connection before send a new request.
  // This will free the socket on the WiFi shield
  client.stop();
  String request = "GET /Final/udp.php?" + String(message) + " HTTP/1.1";
  
  // if there's a successful connection:
  if (client.connect(server, 80)) {
    Serial.println("connecting...");
    // send the HTTP GET request:
    requestValues(2);
    Serial.println(request);
    client.println(request);
    client.println("Host: www.arduino.cc");
    client.println("User-Agent: arduino-ethernet");
    client.println("Connection: close");
    client.println();
    //memset(&message[0], 0, sizeof(message));
    // note the time that the connection was made:
    lastConnectionTime = millis();
  } else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
  }
}
void requestValues(int slave)
{ 
  int i = 0;
  
  // Request value from slave
  Wire.requestFrom(slave, 100);
  
  while(0 < Wire.available())
  {
    str = Wire.read();
    if(str != char(255)){
    message[i] = str;
    
    i++;
  }
  }
  
  message[i] = '\0';
}
