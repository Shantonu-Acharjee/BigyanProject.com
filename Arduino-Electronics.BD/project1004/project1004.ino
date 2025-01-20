/*GND..............GND
+5V..............+5V
8................Enable (EN)
9................Direction (DIR)
10...............STEP (CLK)
*/


/* Pins used for control signals */
#define ENABLE 8
#define DIRECTION 9
#define STEP 10
#define SLEEP 7

#define FORWARD HIGH
#define REVERSE LOW

/* Change this values to alter the clock speed */
#define SPEED 1

void setup() 
{
  pinMode(ENABLE, OUTPUT);
  pinMode(DIRECTION, OUTPUT);
  pinMode(STEP, OUTPUT);
  pinMode(SLEEP, OUTPUT);

  /* Pull the enable pin low to enable the driver */
  digitalWrite(ENABLE, LOW);
  digitalWrite(SLEEP, HIGH);
}


void loop() 
{
  /* The the rotational direction to the forward direction */
  digitalWrite(DIRECTION, FORWARD);

  /* Keep stepping the motor in an infinite loop */
  for(int x = 0; x < 100; x++) {
    digitalWrite(STEP, HIGH);   
    delay(SPEED);              
    digitalWrite(STEP, LOW);    
    delay(SPEED);            
  }
  digitalWrite(SLEEP, LOW);
  delay(300000);
  digitalWrite(SLEEP, HIGH);
}
