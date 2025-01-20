 /* this code is written by Shawon
for more info check my site https://www.electronics.com.bd
*/
#include <Servo.h> // include Servo library 

Servo horizontal; // horizontal/Top servo
int servoh = 90; // stand horizontal servo

Servo vertical; // vertical/Bottom servo 
int servov = 90; // stand vertical servo

// LDR pin connections
int ldrlt = 0; //LDR top left
int ldrrt = 1; //LDR top rigt
int ldrld = 3; //LDR down left
int ldrrd = 2; //ldr down rigt
void setup()
{
  Serial.begin(115200);
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
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
  int ldr = analogRead(4);
  //Serial.print("ldr");
  //Serial.print("=");
  //Serial.print(ldr);
  //Serial.println();
  if(ldr < 20)
  {
    digitalWrite(2,LOW);
    digitalWrite(3,LOW);
    digitalWrite(4,LOW);
    digitalWrite(5,LOW);
  }
  else{    
    digitalWrite(2,HIGH);
    digitalWrite(3,HIGH);
    digitalWrite(4,HIGH);
    digitalWrite(5,HIGH);
  }
  int dtime = 50;//analogRead(4)/20; // read potentiometers
  int tol = 50;//analogRead(5)/4;
  
  int avt = (lt + rt) / 2; // average value top
  int avd = (ld + rd) / 2; // average value down
  int avl = (lt + ld) / 2; // average value left
  int avr = (rt + rd) / 2; // average value right
  Serial.println("lt");
  Serial.print(lt);
  Serial.print("=");
  Serial.print("rt");
  Serial.print(rt);
  Serial.print("=");
  Serial.print("ld");
  Serial.print(ld);
  Serial.print("=");
  Serial.print("rd");
  Serial.print(rd);
  Serial.print("=");
  //delay(1000);
  int dvert = avt - avd; // check the diffirence of up and down
  int dhoriz = avl - avr;// check the diffirence og left and rigt
  
  if (-1*tol > dvert || dvert > tol) // check if the diffirence is in the tolerance else change vertical angle
  {
  if (avt > avd)
  {
  servov = ++servov;
  if (servov > 180)
  {
  servov = 180;
  }
  }
  else if (avt < avd)
  {
  servov= --servov;
  if (servov < 0)
  {
  servov = 0;
  }
  }
  vertical.write(servov);
  }
  
  if (-1*tol > dhoriz || dhoriz > tol) // check if the diffirence is in the tolerance else change horizontal angle
  {
  if (avl > avr)
  {
  servoh = --servoh;
  if (servoh < 40)
  {
  servoh = 40;
  }
  }
  else if (avl < avr)
  {
  servoh = ++servoh;
  if (servoh > 140)
  {
  servoh = 140;
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
