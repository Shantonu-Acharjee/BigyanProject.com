#define PIR_sensor1 2
#define PIR_sensor2 4
#define IR_sensor1 A0
#define IR_sensor2 A1
#define buzz 6
#define PIR_led 7
#define IR_led 8
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;
int PIR_flag = 0;

void setup() {
  Serial.begin(9600);
  pinMode(PIR_sensor1, INPUT);
  pinMode(PIR_sensor2, INPUT);
  pinMode(buzz, OUTPUT);
  pinMode(PIR_led, OUTPUT);
  pinMode(IR_led, OUTPUT);
}

void loop() {
  if(digitalRead(PIR_sensor1) || digitalRead(PIR_sensor2))
  {
    Serial.println("PIR");
    Serial.println("Human");
    digitalWrite(PIR_led,HIGH);
    one();
    one();
    one();
    one();
    one();
    one();
    one();
    one();
    one();
    one();
    one();
    one();
    one();
    //delay(5000);
    noTone(buzz);
    digitalWrite(PIR_led,LOW);
  }
  else if(analogRead(IR_sensor1) < 940 || analogRead(IR_sensor2) < 940)
  {
    Serial.println("veh");
    digitalWrite(IR_led,HIGH);
    three();
    three();
    three();
    three();
    three();
    three();
    three();
    three();
    three();
    noTone(buzz);
    digitalWrite(IR_led,LOW);
  }
//delay(1000);
}
void one() {         //This function produces the 1st siren sound with ON/OFF led transition.
  // Whoop up
  for(int hz = 440; hz < 1000; hz+=25){
    tone(buzz, hz, 50);
    delay(5);
  }
  // Whoop down
  for(int hz = 1000; hz > 440; hz-=25){
    tone(buzz, hz, 50);
    delay(5);
  }
}
 void three() {              //This function produces the 3rd siren(AMBULANCE) sound with led transition.
  tone(buzz,440,200);
    delay(100);
    loopF(5,10,20);
    loopR(10,5,20);
  noTone(buzz);
  
  tone(buzz,494,500);
  delay(300);
  noTone(buzz);
  tone(buzz,523,300);
   delay(300);
  noTone(buzz);
}
 // SOME EXTRA FUNCTIONS OTHER THAN THE SIREN TONES
    void loopF(int spin,int epin,int dela){    //loopF can blink the led in forward direction so spin must be lower than epin.
    for(int i=spin;i<=epin;i++){
    //digitalWrite(i,HIGH);
    delay(dela);
    low();
    if(spin==epin){
    spin=3;
    epin=12;}
    }}
  void loopR(int epin,int spin,int dela){      //loopR can blink the led in reverse/backward direction so epin must be lower than spin.
    for(int i=epin;i>=spin;i--){
    //digitalWrite(i,HIGH);
    delay(dela);
    low();
    if(spin==epin){
    spin=3;
    epin=12;}
    }}
  void low(){          
    }
