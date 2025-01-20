#include <Wire.h>
#include <Adafruit_SSD1306.h>

// Pin Definitions
const int irSensorPins[8] = {2, 3, 4, 5, 6, 7, 8, 9};  // IR sensor pins
const int motor1PWM = 10;  // PWM pin for Motor1 (TB6612FNG)
const int motor2PWM = 11;  // PWM pin for Motor2 (TB6612FNG)
const int motor1In1 = A0;  // Motor1 control pin 1
const int motor1In2 = A1;  // Motor1 control pin 2
const int motor2In1 = A2;  // Motor2 control pin 1
const int motor2In2 = A3;  // Motor2 control pin 2
const int buttonPin1 = 12; // Push button 1
const int buttonPin2 = 13; // Push button 2
const int trigPin = A6;    // Ultrasonic trigger pin
const int echoPin = A7;    // Ultrasonic echo pin

// OLED display
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// PID constants
float kp = 0.6;
float ki = 0.0;
float kd = 0.3;
int baseSpeed = 150;
int maxSpeed = 255;
float error = 0, previousError = 0, pidOutput = 0, integral = 0, derivative = 0;

// Line Sensor variables
int sensorValues[8];
int position = 0;  // Center of the line

void setup() {
  // Motor pins
  pinMode(motor1In1, OUTPUT);
  pinMode(motor1In2, OUTPUT);
  pinMode(motor2In1, OUTPUT);
  pinMode(motor2In2, OUTPUT);
  pinMode(motor1PWM, OUTPUT);
  pinMode(motor2PWM, OUTPUT);

  // IR Sensor pins
  for (int i = 0; i < 8; i++) {
    pinMode(irSensorPins[i], INPUT);
  }

  // Push button pins
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);

  // Ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.display();

  // Display initial message
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Line Following Robot");
  display.display();

  Serial.begin(9600);  // Start serial for debugging
}

void loop() {
  // Read IR Sensors
  readIRSensors();

  // Calculate PID values
  calculatePID();

  // Motor Control
  motorControl();

  // Display info on OLED
  displayOLED();
}

void readIRSensors() {
  position = 0;
  int totalValue = 0;

  for (int i = 0; i < 8; i++) {
    sensorValues[i] = digitalRead(irSensorPins[i]);
    if (sensorValues[i] == LOW) {  // Assuming LOW means line detected
      position += i * 1000;  // Weight the sensor position
      totalValue += 1;
    }
  }

  if (totalValue != 0) {
    position = position / totalValue;  // Find the average position
  } else {
    // Line is lost, act accordingly
    position = -1;  // You can modify this if you want to stop or adjust the robot's behavior when the line is lost
  }
}

void calculatePID() {
  if (position != -1) {  // If line is detected
    error = position - 3500;  // Centered value is 3500

    // PID calculations
    integral += error;
    derivative = error - previousError;
    pidOutput = (kp * error) + (ki * integral) + (kd * derivative);

    previousError = error;
  } else {
    pidOutput = 0;  // Stop motors if the line is lost
  }
}

void motorControl() {
  int motor1Speed = baseSpeed + pidOutput;
  int motor2Speed = baseSpeed - pidOutput;

  motor1Speed = constrain(motor1Speed, 0, maxSpeed);
  motor2Speed = constrain(motor2Speed, 0, maxSpeed);

  // Motor1 Control
  if (motor1Speed > 0) {
    analogWrite(motor1PWM, motor1Speed);
    digitalWrite(motor1In1, HIGH);
    digitalWrite(motor1In2, LOW);
  } else {
    analogWrite(motor1PWM, -motor1Speed);
    digitalWrite(motor1In1, LOW);
    digitalWrite(motor1In2, HIGH);
  }

  // Motor2 Control
  if (motor2Speed > 0) {
    analogWrite(motor2PWM, motor2Speed);
    digitalWrite(motor2In1, HIGH);
    digitalWrite(motor2In2, LOW);
  } else {
    analogWrite(motor2PWM, -motor2Speed);
    digitalWrite(motor2In1, LOW);
    digitalWrite(motor2In2, HIGH);
  }
}

void displayOLED() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Line Follower PID");
  display.print("Error: ");
  display.println(error);
  display.print("PID Output: ");
  display.println(pidOutput);
  display.display();
}

