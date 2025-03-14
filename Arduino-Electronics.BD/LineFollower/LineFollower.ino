int ENA = 6;  //ENA connected to digital pin 3
int ENB = 3;  //ENB connected to digital pin 9
int MOTOR_A1 = 9; // MOTOR_A1 connected to digital pin 4
int MOTOR_A2 = 8; // MOTOR_A2 connected to digital pin 5
int MOTOR_B1 = 5; // MOTOR_B1 connected to digital pin 6
int MOTOR_B2 = 7; // MOTOR_B2 connected to digital pin 7
int RIGHT_sensor = 4; // RIGHT sensor connected to analog pin A0
int LEFT_sensor = 2;  // LEFT sensor connected to analog pin A1

int ENAV = 90;
int ENBV = 90;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);  
  pinMode(ENA, OUTPUT); // initialize ENA pin as an output
  pinMode(ENB, OUTPUT); // initialize ENB pin as an output
  pinMode(MOTOR_A1, OUTPUT); // initialize MOTOR_A1 pin as an output
  pinMode(MOTOR_A2, OUTPUT); // initialize MOTOR_A2 pin as an output
  pinMode(MOTOR_B1, OUTPUT); // initialize MOTOR_B1 pin as an output
  pinMode(MOTOR_B2, OUTPUT); // initialize MOTOR_B2 pin as an output
  pinMode(RIGHT_sensor, INPUT); // initialize RIGHT pin as an input
  pinMode(LEFT_sensor, INPUT);  // initialize LEFT pin as an input
}

void loop() {
   if (!digitalRead(RIGHT_sensor) && !digitalRead(LEFT_sensor)) {
      moveForward();
    }else if (!digitalRead(RIGHT_sensor) && digitalRead(LEFT_sensor)) {
      turnLeft();
      //delay(100);
    }else if (digitalRead(RIGHT_sensor) && !digitalRead(LEFT_sensor)) {
      turnRight(); 
      //delay(100);
    }else if (digitalRead(RIGHT_sensor) && digitalRead(LEFT_sensor)) { 
      moveForward();
    }
}
void Stop() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(MOTOR_A1, LOW);
  digitalWrite(MOTOR_A2, LOW);
  digitalWrite(MOTOR_B1, LOW);
  digitalWrite(MOTOR_B2, LOW);
}

void turnRight() {
  analogWrite(ENA, ENAV);
  analogWrite(ENB, ENBV);
  digitalWrite(MOTOR_A1, HIGH);
  digitalWrite(MOTOR_A2, LOW);
  digitalWrite(MOTOR_B1, HIGH);
  digitalWrite(MOTOR_B2, LOW);
  
}

void turnLeft() {
  
  analogWrite(ENA, ENAV);
  analogWrite(ENB, ENBV);
  digitalWrite(MOTOR_A1, LOW);
  digitalWrite(MOTOR_A2, HIGH);
  digitalWrite(MOTOR_B1, LOW);
  digitalWrite(MOTOR_B2, HIGH);
}

void moveForward() {
  analogWrite(ENA, ENAV);
  analogWrite(ENB, ENBV);
  digitalWrite(MOTOR_A1, LOW);
  digitalWrite(MOTOR_A2, HIGH);
  digitalWrite(MOTOR_B1, HIGH);
  digitalWrite(MOTOR_B2, LOW);
}
void moveBackward() {
  analogWrite(ENA, ENAV);
  analogWrite(ENB, ENBV);
  digitalWrite(MOTOR_A1, HIGH);
  digitalWrite(MOTOR_A2, LOW);
  digitalWrite(MOTOR_B1, LOW);
  digitalWrite(MOTOR_B2, HIGH);
}
