 /* this code is written by geobruce
for more info check my site http://xprobe.net
*/
#include <Servo.h> // include Servo library 

Servo horizontal; // horizontal servo
int servoh = 90; // stand horizontal servo

Servo vertical; // vertical servo 
int servov = 90; // stand vertical servo

// LDR pin connections
int ldrlt = A1; //LDR top left
int ldrrt = A0; //LDR top rigt
int ldrld = A2; //LDR down left
int ldrrd = A3; //ldr down rigt

void setup()
{
  Serial.begin(9600);
  horizontal.attach(9); 
  vertical.attach(10);
  vertical.write(90);
  horizontal.write(90);
}

void loop() 
{
  int lt = analogRead(ldrlt); // top left
  int rt = analogRead(ldrrt); // top right
  int ld = analogRead(ldrld); // down left
  int rd = analogRead(ldrrd); // down rigt

  int dtime = analogRead(4)/20; // read potentiometers
  int tol = analogRead(5)/4;
  
  int avt = (lt + rt) / 2; // average value top
  int avd = (ld + rd) / 2; // average value down
  int avl = (lt + ld) / 2; // average value left
  int avr = (rt + rd) / 2; // average value right
  Serial.print("lt");
  Serial.print(lt);
  Serial.println("=");
  Serial.print("rt");
  Serial.print(rt);
  Serial.println("=");
  Serial.print("ld");
  Serial.print(ld);
  Serial.println("=");
  Serial.print("rd");
  Serial.print(rd);
  Serial.println("=");
  delay(1000);
  int dvert = avt - avd; // check the diffirence of up and down
  int dhoriz = avl - avr;// check the diffirence og left and rigt
  
  if (-1*tol > dvert || dvert > tol) // check if the diffirence is in the tolerance else change vertical angle
  {
  if (avt > avd)
  {
  servov = ++servov;
  if (servov > 160)
  {
  servov = 160;
  }
  }
  else if (avt < avd)
  {
  servov= --servov;
  if (servov < 20)
  {
  servov = 20;
  }
  }
  vertical.write(servov);
  }
  
  if (-1*tol > dhoriz || dhoriz > tol) // check if the diffirence is in the tolerance else change horizontal angle
  {
  if (avl > avr)
  {
  servoh = --servoh;
  if (servoh < 20)
  {
  servoh = 20;
  }
  }
  else if (avl < avr)
  {
  servoh = ++servoh;
  if (servoh > 160)
  {
  servoh = 160;
  }
  }
  else if (avl == avr)
  {
  // nothing
  }
  horizontal.write(servoh);
  }
  delay(dtime);
} 
