String textMessage;
const int red = 12;
const int green = 11;
const int blue = 10;
void setup() {
   // Set leds as OUTPUT
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  // By default the leds is off
  digitalWrite(red, LOW);
  digitalWrite(green, LOW);
  digitalWrite(blue, LOW);
  
  // Initializing serial commmunication
  Serial.begin(19200); 
  
  // Give time to your GSM shield log on to network
  delay(20000);
  // AT command to set SIM900 to SMS mode
  Serial.print("AT+CMGF=1\r"); 
  delay(100);
}
void loop(){
  if(Serial.available()>0){
    textMessage = Serial.readString();
   textMessage.toUpperCase();   
    delay(10);
  } 
  if(textMessage.indexOf("REDON")>=0){
    // Turn on red
    digitalWrite(red, HIGH);  
    textMessage = "";   
  }
  if(textMessage.indexOf("REDOFF")>=0){
    // Turn off red
    digitalWrite(red, LOW);
    textMessage = ""; 
  }
  if(textMessage.indexOf("GREENON")>=0){
    // Turn on green
    digitalWrite(green, HIGH);  
    textMessage = "";   
  }
  if(textMessage.indexOf("GREENOFF")>=0){
    // Turn off green
    digitalWrite(green, LOW);
    textMessage = ""; 
  }
  if(textMessage.indexOf("BLUEON")>=0){
    // Turn on blue 
    digitalWrite(blue, HIGH); 
    textMessage = "";   
  }
  if(textMessage.indexOf("BLUEOFF")>=0){
    // Turn off Blue 
    digitalWrite(blue, LOW);
    textMessage = ""; 
  }
   if(textMessage.indexOf("ALLON")>=0){
    // Turn on all leds
    digitalWrite(blue, HIGH);
    digitalWrite(green, HIGH); 
    digitalWrite(red, HIGH);  
    textMessage = "";   
  }
  if(textMessage.indexOf("ALLOFF")>=0){
    // Turn off all leds
    digitalWrite(blue, LOW);
    digitalWrite(red, LOW);
    digitalWrite(green, LOW);
    textMessage = ""; 
  }
}
