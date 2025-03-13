char Incoming_value = 0;

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    Incoming_value = Serial.read(); // Read a single character
    Serial.println(Incoming_value); // Print the character

    if (Incoming_value == 'F') {
      digitalWrite(13, HIGH); // Turn on LED
    } else if (Incoming_value == 'B') {
      digitalWrite(13, LOW); // Turn off LED
    }
  }
}