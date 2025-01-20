// defines pins numbers
const int trigPin = 9;
const int echoPin = 10;
const int buzzerPin = 5;

// defines variables
long duration;
int distance;

void setup() {
//pinMode(4, OUTPUT); // Sets the trigPin as an Output
//digitalWrite(4, LOW);
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
pinMode(buzzerPin, OUTPUT); // Sets the trigPin as an Output
digitalWrite(buzzerPin, HIGH);
Serial.begin(9600); // Starts the serial communication
}

void loop() {
// Clears the trigPin
digitalWrite(trigPin, LOW);
delayMicroseconds(2);

// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);

// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);

// Calculating the distance
distance= duration*0.034/2;

// Prints the distance on the Serial Monitor
if (distance <= 50 && distance >= 0) {
// Buzz
digitalWrite(buzzerPin, LOW);
} else {
// Don't buzz
digitalWrite(buzzerPin, HIGH);
}
Serial.print("Distance: ");
Serial.println(distance);
}
