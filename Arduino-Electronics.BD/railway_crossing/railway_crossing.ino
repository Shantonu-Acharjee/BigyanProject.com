#include <Servo.h>
#define trigPin1 4
#define echoPin1 5
#define trigPin2 3
#define echoPin2 2
#define buzzer 13
int pos = 0;
int flag1 = 0;
int flag2 = 0;
Servo myservo1;
Servo myservo2;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);
  myservo1.attach(6);
  myservo2.attach(9);
  myservo1.write(90);
  myservo2.write(90);
  delay(2000);
}

void loop() {
  long duration1, distance1;
  long duration2, distance2;
  digitalWrite(trigPin1, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = (duration1/2) / 29.1;
  digitalWrite(trigPin2, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = (duration2/2) / 29.1;
  Serial.print("flag1 " + String(flag1));
  Serial.print("=flag2 " + String(flag2));
  Serial.print("=distance " + String(distance1));
  Serial.println("=distance " + String(distance2));
  if (distance1 < 7 && flag1 == 0 && flag2 == 0 && distance1 != 0) {
    flag1 = 1;
    digitalWrite(buzzer, HIGH);
    for(pos = 90; pos < 180; pos += 1)  // goes from 0 degrees to 180 degrees
    {                                  // in steps of 1 degree
      myservo1.write(pos);              // tell servo to go to position in variable 'pos'
      myservo2.write(pos);
      delay(5);                       // waits 15ms for the servo to reach the position
    }
    distance1 = 7;
  }
  if (distance2 < 7 && flag1 == 1 && flag2 == 0 && distance2 != 0) {
    flag1 = 0;
    digitalWrite(buzzer, LOW);
    for(pos = 180; pos>=90; pos-=1)  // goes from 0 degrees to 180 degrees
    {                                  // in steps of 1 degree
      myservo1.write(pos);
      myservo2.write(pos);              // tell servo to go to position in variable 'pos'
      delay(5);                       // waits 15ms for the servo to reach the position
    }
    distance2 = 7;
  }
  if (distance2 < 7 && flag2 == 0 && flag1 == 0 && distance2 != 0) {
    flag2 = 1;
    digitalWrite(buzzer, HIGH);
    for(pos = 90; pos < 180; pos += 1)  // goes from 0 degrees to 180 degrees
    {                                  // in steps of 1 degree
      myservo1.write(pos);              // tell servo to go to position in variable 'pos'
      myservo2.write(pos);
      delay(5);                       // waits 15ms for the servo to reach the position
    }
    distance2 = 7;
  }
  if (distance1 < 7 && flag2 == 1 && flag1 == 0 && distance1 != 0) {
    flag2 = 0;
    digitalWrite(buzzer, LOW);
    for(pos = 180; pos>=90; pos-=1)  // goes from 0 degrees to 180 degrees
    {                                  // in steps of 1 degree
      myservo1.write(pos);
      myservo2.write(pos);              // tell servo to go to position in variable 'pos'
      delay(5);                       // waits 15ms for the servo to reach the position
    }
    distance1 = 7;
  }
}
