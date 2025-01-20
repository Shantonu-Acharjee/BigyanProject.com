#include "IRremote.h"

/*-----( Declare Constants )-----*/
int receiver = 11; // pin 1 of IR receiver to Arduino digital pin 11
int Light_PIN = 10;
int Fan_PIN = 9;
/*-----( Declare objects )-----*/
IRrecv irrecv(receiver);           // create instance of 'irrecv'
decode_results results;            // create instance of 'decode_results'
/*-----( Declare Variables )-----*/


void setup()   /*----( SETUP: RUNS ONCE )----*/
{
  Serial.begin(9600);
  pinMode(Light_PIN, OUTPUT);
  pinMode(Fan_PIN, OUTPUT);
  Serial.println("IR Receiver Raw Data + Button Decode Test");
  irrecv.enableIRIn(); // Start the receiver

}/*--(end setup )---*/


void loop()   /*----( LOOP: RUNS CONSTANTLY )----*/
{
  if (irrecv.decode(&results)) // have we received an IR signal?

  {
    translateIR(); 
    irrecv.resume(); // receive the next value
  }  
}/* --(end main loop )-- */

/*-----( Declare User-written Functions )-----*/
void translateIR() // takes action based on IR code received

// describing IR codes 

{
Serial.println(results.value); 

  switch(results.value)

  {
  case 4146012074:  
    on(Fan_PIN); 
    break;

  case 3560376831:  
    off(Fan_PIN); 
    break;

  case 3711226650:  
    on(Light_PIN); 
    break;

  case 4191748330:  
    off(Light_PIN); 
    break;

  default: 
    Serial.println(" other button   ");

  }

  delay(500);


} //END translateIR

void on(int Pin)
{
  digitalWrite(Pin, HIGH);
}
 
void off(int Pin)
{
  digitalWrite(Pin, LOW);
}

/* ( THE END ) */
