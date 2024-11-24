
const int irSensorPin = A3;  // IR sensor connected to analog pin A0
int irValue = 0;  // Variable to store the analog value

void setup() {
  // Start the serial communication to display the sensor reading
  Serial.begin(9600);
}

void loop() {
  // Read the analog value from the IR sensor
  irValue = analogRead(irSensorPin);
  
  // Print the IR sensor value to the Serial Monitor
  Serial.print("IR Sensor Value: ");
  Serial.println(irValue);
  
  // Add a small delay between readings
  delay(500);  // 500 ms delay between readings
}
