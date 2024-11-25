#define TRIG_PIN 13  // Trig pin connected to GPIO 13
#define ECHO_PIN 12  // Echo pin connected to GPIO 12

void setup() {
  Serial.begin(115200);        // Start serial communication
  pinMode(TRIG_PIN, OUTPUT);   // Set TRIG_PIN as output
  pinMode(ECHO_PIN, INPUT);    // Set ECHO_PIN as input
  Serial.println("Ultrasonic Sensor Test Initialized");
}

void loop() {
  long duration;
  float distance;

  // Send a 10-microsecond pulse to trigger the sensor
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Measure the time for the echo pulse
  duration = pulseIn(ECHO_PIN, HIGH);

  // Calculate the distance in centimeters
  distance = duration * 0.034 / 2;  // Speed of sound: 343 m/s or 0.034 cm/µs

  // Print the distance
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(500); // Wait for half a second before the next reading
}
