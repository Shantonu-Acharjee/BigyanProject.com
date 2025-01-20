const int DIR = 12;
const int STEP = 14;
const int button = 5;
const int  steps_per_rev = 6000;


// Timer: Auxiliary variables
unsigned long now = millis();
unsigned long lastTrigger = 0;
boolean startTimer = false;

// Checks if motion was detected, sets LED HIGH and starts a timer
ICACHE_RAM_ATTR void detectsMovement() {
  Serial.println("Interrupt!!!");
  startTimer = true;
  lastTrigger = millis();
}

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);
  pinMode(STEP, OUTPUT);
  pinMode(DIR, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  digitalWrite(DIR, false);
  // Set motionSensor pin as interrupt, assign interrupt function and set RISING mode
  attachInterrupt(digitalPinToInterrupt(button), detectsMovement, RISING);
}

void loop() {
  if(startTimer == false){
    digitalWrite(STEP, HIGH);
    delayMicroseconds(500);
    digitalWrite(STEP, LOW);
    delayMicroseconds(500);
  }
  if(startTimer == true){
    delay(1000);
    digitalWrite(DIR, startTimer);
    startTimer = false;
  }
  
}
