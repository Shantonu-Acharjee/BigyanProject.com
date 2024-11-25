#define ANALOG_PIN 34  // Define the ADC pin (e.g., GPIO 34 is ADC1_CH6)

void setup() {
  Serial.begin(115200);        // Initialize Serial Monitor
  pinMode(ANALOG_PIN, INPUT);  // Set the analog pin as input
  Serial.println("Analog Read Test Initialized");
}

void loop() {
  int analogValue = analogRead(ANALOG_PIN);  // Read the analog value
  Serial.print("Analog Value: ");
  Serial.println(analogValue);              // Print the value to Serial Monitor
  
  delay(500); // Wait 500ms before the next reading
}
