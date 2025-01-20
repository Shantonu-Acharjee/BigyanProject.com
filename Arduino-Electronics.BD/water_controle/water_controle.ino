/*
 * Created by Electronics.Com.BD
 * Project: Water Controle V1.0
 */

#define SEN_1 2 //Sensor 1 connected to digital pin 2
#define SEN_2 3 //Sensor 2 connected to digital pin 3
#define SEN_3 4 //Sensor 3 connected to digital pin 4
#define SEN_4 5 //Sensor 4 connected to digital pin 5

#define SEG_1 9 //Segment 1 connected to digital pin 9
#define SEG_2 10 //Segment 2 connected to digital pin 10
#define SEG_3 11 //Segment 3 connected to digital pin 11
#define SEG_4 12 //Segment 4 connected to digital pin 12
#define JUM_1 13 //Jumper 1 connected to digital pin 13
#define JUM_2 7 //Jumper 2 connected to digital pin 7
#define JUM_3 8 //Jumper 3 connected to digital pin 8

#define Motor 6 //Motor/Output connect to digital pin 6
 
int motor_status = 0;
int SEN_1_State = 0;
int SEN_2_State = 0;
int SEN_3_State = 0;
int SEN_4_State = 0;

void setup() {
  pinMode(SEN_1, INPUT);
  pinMode(SEN_2, INPUT);
  pinMode(SEN_3, INPUT);
  pinMode(SEN_4, INPUT);
  pinMode(JUM_1, INPUT);
  pinMode(JUM_2, INPUT);
  pinMode(JUM_3, INPUT);
  pinMode(SEG_1, OUTPUT);
  pinMode(SEG_2, OUTPUT);
  pinMode(SEG_3, OUTPUT);
  pinMode(SEG_4, OUTPUT);
  pinMode(Motor, OUTPUT);
  Serial.begin(9600);
  delay(2000);
}

void loop() {
  SEN_1_State = digitalRead(SEN_1);
  SEN_2_State = digitalRead(SEN_2);
  SEN_3_State = digitalRead(SEN_3);
  SEN_4_State = digitalRead(SEN_4);
  Serial.print(SEN_1_State);
  Serial.print(",");
  Serial.print(SEN_2_State);
  Serial.print(",");
  Serial.print(SEN_3_State);
  Serial.print(",");
  Serial.print(SEN_4_State);
  Serial.println(",");
  if(digitalRead(SEN_1) == HIGH && digitalRead(SEN_2) == HIGH && digitalRead(SEN_3) == HIGH && digitalRead(SEN_4) == HIGH)
  {
    //Serial.println("All sen high");
    digitalWrite(Motor, LOW);
    digitalWrite(SEG_1, HIGH);
    digitalWrite(SEG_2, HIGH);
    digitalWrite(SEG_3, HIGH);
  }
  else if(digitalRead(SEN_1) == LOW && digitalRead(SEN_2) == HIGH && digitalRead(SEN_3) == HIGH && digitalRead(SEN_4) == HIGH)
  {
    //Serial.println("Sen1 low");
    digitalWrite(Motor, LOW);
    digitalWrite(SEG_1, HIGH);
    digitalWrite(SEG_2, HIGH);
    digitalWrite(SEG_3, HIGH);
  }
  else if(digitalRead(SEN_1) == LOW && digitalRead(SEN_2) == LOW && digitalRead(SEN_3) == HIGH && digitalRead(SEN_4) == HIGH)
  {
    //Serial.println("Sen2 low");
    digitalWrite(Motor, HIGH);
    digitalWrite(SEG_1, LOW);
    digitalWrite(SEG_2, HIGH);
    digitalWrite(SEG_3, HIGH);
  }
  else if(digitalRead(SEN_1) == LOW && digitalRead(SEN_2) == LOW && digitalRead(SEN_3) == LOW && digitalRead(SEN_4) == HIGH)
  {
    //Serial.println("Sen3 low");
    digitalWrite(Motor, HIGH);
    digitalWrite(SEG_1, LOW);
    digitalWrite(SEG_2, LOW);
    digitalWrite(SEG_3, HIGH);
  }
  else if(digitalRead(SEN_1) == LOW && digitalRead(SEN_2) == LOW && digitalRead(SEN_3) == LOW && digitalRead(SEN_4) == LOW)
  {
    //Serial.println("Sen4 low");
    digitalWrite(Motor, HIGH);
    digitalWrite(SEG_1, LOW);
    digitalWrite(SEG_2, LOW);
    digitalWrite(SEG_3, LOW);
  }

}
