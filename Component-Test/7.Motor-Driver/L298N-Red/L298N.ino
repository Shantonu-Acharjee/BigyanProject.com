// Define motor control pins
const int ENA1 = 5;    // PWM pin to control speed (ENA1ble A)
const int ENA2 = 6;    // PWM pin to control speed (ENA1ble A)

const int IN1 = 8;    // Motor control pin 1
const int IN2 = 9;    // Motor control pin 2
const int IN3 = 10;    // Motor control pin 1
const int IN4 = 11;    // Motor control pin 2


void setup() {
  // Set motor control pins as outputs
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(ENA1, OUTPUT);
  pinMode(ENA2, OUTPUT);
}

void loop() {
  //Forward();
 // delay(2000);
 // Backword();
 // delay(2000);
 Right();
}





void Forward(){
  analogWrite(ENA1, 250);
  analogWrite(ENA2, 250);
  digitalWrite(IN1, HIGH);   // Set IN1 to HIGH
  digitalWrite(IN2, LOW);    // Set IN2 to LOW
  digitalWrite(IN3, LOW);   // Set IN1 to HIGH
  digitalWrite(IN4, HIGH);    // Set IN2 to LOW  
  
}



void Backword(){
  analogWrite(ENA1, 250);
  analogWrite(ENA2, 250);
  digitalWrite(IN1, LOW);   // Set IN1 to HIGH
  digitalWrite(IN2, HIGH);    // Set IN2 to LOW
  digitalWrite(IN3, HIGH);   // Set IN1 to HIGH
  digitalWrite(IN4, LOW);    // Set IN2 to LOW  
  
}



void Left(){
  analogWrite(ENA1, 130);
  analogWrite(ENA2, 250);
  digitalWrite(IN1, HIGH);   // Set IN1 to HIGH
  digitalWrite(IN2, LOW);    // Set IN2 to LOW
  digitalWrite(IN3, LOW);   // Set IN1 to HIGH
  digitalWrite(IN4, HIGH);    // Set IN2 to LOW  
  
}


void Right(){
  analogWrite(ENA1, 250);
  analogWrite(ENA2, 130);
  digitalWrite(IN1, HIGH);   // Set IN1 to HIGH
  digitalWrite(IN2, LOW);    // Set IN2 to LOW
  digitalWrite(IN3, LOW);   // Set IN1 to HIGH
  digitalWrite(IN4, HIGH);    // Set IN2 to LOW  
  
}


