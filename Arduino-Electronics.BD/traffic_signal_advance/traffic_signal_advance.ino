#include<TimerOne.h>

int post1[] = {7, 8, 9};
int post2[] = {42, 44, 46};
int post3[] = {53, 51, 49};
int post4[] = {14, 2, 3};

int sensor1[] = {10, 11, 12};
int sensor2[] = {48, 50, 52};
int sensor3[] = {47, 45, 43};
int sensor4[] = {4, 5, 6};

int redDelay = 5000;
int yellowDelay = 2000;       

volatile int S1, S2, S3, S4;           // Variables for storing the distance covered

void setup(){
  Serial.begin(115200);
  Timer1.initialize(100000);  //Begin using the timer. This function must be called first. "microseconds" is the period of time the timer takes.
  Timer1.attachInterrupt(softInterr); //Run a function each time the timer period finishes.

  // Declaring LED pins as output
  for(int i=0; i<3; i++){
    pinMode(post1[i], OUTPUT);
    pinMode(post2[i], OUTPUT);
    pinMode(post3[i], OUTPUT);
    pinMode(post4[i], OUTPUT);
  }

  // Declaring IR sensor pins as input
  for(int i=0; i<3; i++){
    pinMode(sensor1[i], INPUT);
    pinMode(sensor2[i], INPUT);
    pinMode(sensor3[i], INPUT);
    pinMode(sensor4[i], INPUT);
  } 
}

void loop()
{
  // If there are vehicles at signal 1
  if(S1!=3)
  {
    post1Function();
  }

  // If there are vehicles at signal 2
  if(S2!=3)
  {
    post2Function();
  }

  // If there are vehicles at signal 3
  if(S3!=3)
  {
    post3Function();
  }

  // If there are vehicles at signal 4
  if(S4!=3)
  {
    post4Function();
  }
}

// This is interrupt function and it will run each time the timer period finishes. The timer period is set at 100 milli seconds.
void softInterr()
{
  S1 = 0;
  S2 = 0;
  S3 = 0;
  S4 = 0;
  for(int i=0; i<3; i++){
    S1 = S1+digitalRead(sensor1[i]);
    S2 = S2+digitalRead(sensor2[i]);
    S3 = S3+digitalRead(sensor3[i]);
    S4 = S4+digitalRead(sensor4[i]);
  }

  // Print distance values on serial monitor for debugging
  Serial.print("S1: ");
  Serial.print(S1);
  Serial.print("  S2: ");
  Serial.print(S2);
  Serial.print("  S3: ");
  Serial.print(S3);
  Serial.print("  S4: ");
  Serial.println(S4);
}

void post1Function()
{
  Serial.println("1");
  low();
  // if there are vehicels at other signals
  if(S2<3 || S3<3 || S4<3)
  {
    // Make Green LED LOW and make yellow LED HIGH for 2 seconds
    digitalWrite(post1[0], LOW);
    digitalWrite(post1[1], HIGH);
    digitalWrite(post1[2], LOW);
    delay(yellowDelay);
  }
  // Make RED LED LOW and make Green HIGH for 5 seconds
  digitalWrite(post1[0], LOW);
  digitalWrite(post1[1], LOW);
  digitalWrite(post1[2], HIGH);
  if(S1==0)
  {
    delay(redDelay+3000);    
  }
  else if(S1==1)
  {
    delay(redDelay+2000);    
  }
  else
  {
    delay(redDelay);
  }
}

void post2Function()
{
  Serial.println("2");
  low();
  if(S1<3 || S3<3 || S4<3)
  {
    digitalWrite(post2[0], LOW);
    digitalWrite(post2[1], HIGH);
    digitalWrite(post2[2], LOW);
    delay(yellowDelay);
  }
  // Make RED LED LOW and make Green HIGH for 5 seconds
  digitalWrite(post2[0], LOW);
  digitalWrite(post2[1], LOW);
  digitalWrite(post2[2], HIGH);
  if(S2==0)
  {
    delay(redDelay+3000);    
  }
  else if(S2==1)
  {
    delay(redDelay+2000);    
  }
  else
  {
    delay(redDelay);
  }
  
}

void post3Function()
{
  Serial.println("3");
  low();
  if(S1<3 || S2<3 || S4<3)
  {
    digitalWrite(post3[0], LOW);
    digitalWrite(post3[1], HIGH);
    digitalWrite(post3[2], LOW);
    delay(yellowDelay);
  }
  // Make RED LED LOW and make Green HIGH for 5 seconds
  digitalWrite(post3[0], LOW);
  digitalWrite(post3[1], LOW);
  digitalWrite(post3[2], HIGH);
  if(S3==0)
  {
    delay(redDelay+3000);    
  }
  else if(S3==1)
  {
    delay(redDelay+2000);    
  }
  else
  {
    delay(redDelay);
  }
 
}

void post4Function()
{
  Serial.println("4");
  low();
  if(S1<3 || S2<3 || S3<3)
  {
    digitalWrite(post4[0], LOW);
    digitalWrite(post4[1], HIGH);
    digitalWrite(post4[2], LOW);
    delay(yellowDelay);
  }
  // Make RED LED LOW and make Green HIGH for 5 seconds
  digitalWrite(post4[0], LOW);
  digitalWrite(post4[1], LOW);
  digitalWrite(post4[2], HIGH);
  if(S4==0)
  {
    delay(redDelay+3000);    
  }
  else if(S4==1)
  {
    delay(redDelay+2000);    
  }
  else
  {
    delay(redDelay);
  }

}

// Function to make all LED's LOW except RED one's.
void low()
{
  for(int i=1; i<3; i++)
  {
    digitalWrite(post1[i], LOW);
    digitalWrite(post2[i], LOW);
    digitalWrite(post3[i], LOW);
    digitalWrite(post4[i], LOW);
  }
  for(int i=0; i<1; i++)
  {
    digitalWrite(post1[i], HIGH);
    digitalWrite(post2[i], HIGH);
    digitalWrite(post3[i], HIGH);
    digitalWrite(post4[i], HIGH);
  }
}
