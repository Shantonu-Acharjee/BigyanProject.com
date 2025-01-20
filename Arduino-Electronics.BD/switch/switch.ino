#define btn0 0
#define btn1 1
#define btn2 2
#define btn3 3
#define out0 13
#define out1 12
#define out2 11
#define out3 10
int state = LOW;  
int state1 = LOW;  
int state2 = LOW;  
int state3 = LOW;      
int reading;      
int reading1;      
int reading2;      
int reading3;          
int previous = LOW;  
int previous1 = LOW;  
int previous2 = LOW;  
int previous3 = LOW;   

long time = 0; 
long time1 = 0; 
long time2 = 0; 
long time3 = 0;         
long debounce = 200;   
void setup() {
  pinMode(btn0, INPUT);
  pinMode(btn1, INPUT);
  pinMode(btn2, INPUT);
  pinMode(btn3, INPUT);
  pinMode(out0, OUTPUT);
  pinMode(out1, OUTPUT);
  pinMode(out2, OUTPUT);
  pinMode(out3, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  reading = digitalRead(btn0);
  /*if (reading == HIGH && millis() - time > debounce) {
    if (state == HIGH)
      state = LOW;
    else
      state = HIGH;

    time = millis();    
  }*/
  digitalWrite(out0, reading);
  Serial.println(reading);
  previous = reading;
  /*reading1 = digitalRead(btn1);
  if (reading1 == HIGH && previous1 == LOW && millis() - time1 > debounce) {
    if (state1 == HIGH)
      state1 = LOW;
    else
      state1 = HIGH;

    time1 = millis();    
  }
  digitalWrite(out1, state1);
  previous1 = reading1; */
}
void toggle_output(int btn,int output)
{
  if (millis() - time > debounce) {
}
}

