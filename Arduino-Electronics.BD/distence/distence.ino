#define trigPin 7
#define echoPin 8
#define solinoied 3

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(solinoied, OUTPUT);
  digitalWrite(solinoied,HIGH);
  Serial.begin(9600);
  delay(3000);
}

void loop() {
  long duration, distance;
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  Serial.println("distance " + String(distance));
  //if (distance < 24 && distance > 18) {  // This is where the LED On/Off happens
  //  digitalWrite(solinoied,LOW); // When the Red condition is met, the Green LED should turn off
  //  delay(5000);
 // }
  //else {
  //  digitalWrite(solinoied,HIGH);
  //}
  delay(500);
}
