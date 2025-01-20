#include <LiquidCrystal.h>
#define in 8
#define out 9
#define fan 10
#define light 11
LiquidCrystal lcd(2,3,4,5,6,7);

int count = 0;
int in_status = 0;
int out_status = 0;
int p_count = 0;
void setup() {
  Serial.begin(9600);
  lcd.begin(16,2);
  pinMode(in, INPUT);
  pinMode(out, INPUT);
  pinMode(fan, OUTPUT);
  pinMode(light, OUTPUT);
  digitalWrite(fan, HIGH);
  digitalWrite(light, HIGH);
  lcd.clear();  
  lcd.setCursor(0,0);
  lcd.print("Person In Room: ");
  lcd.setCursor(0,1);
  lcd.print(count);
}

void loop() {
  int in_value = digitalRead(in);
  int out_value = digitalRead(out);

  if(in_status==0)
  {
    if(in_value == LOW)    // IR value, adjust/change as per your sensor's logic
    {
      in_status=1;
      out_status=0;      
    }
    else if(out_value == LOW)   // IR value, adjust/change as per your sensor's logic
    {
      in_status=1;
      out_status=1;
    }
  }                                                            
  
  if(in_status==1)
  {
    if(out_status==1)
    {
      if(in_value == LOW)    // IR value, adjust/change as per your sensor's logic
      {
        in_status=0;
        if(count<999)
        {     
          count++;
          p_count = 0;
        }  
        while(in_value == LOW) // IR value, adjust/change as per your sensor's logic
        {
          in_value = digitalRead(in);  
        }
       }
    }    
    if(out_status==0)
    {
      if(out_value == LOW)  // IR value, adjust/change as per your sensor's logic
      {
        in_status=0;
        if(count>0)
        {
          count--;
          p_count = 0;
        }        
        while(out_value == LOW) // IR value, adjust/change as per your sensor's logic
        {
          out_value = digitalRead(out);   
        }
      }                 
    }
  }   
  if(count == 0){
    digitalWrite(fan, HIGH);
    digitalWrite(light, HIGH);
    Serial.print("Person In Room: ");
    Serial.print(count);
    Serial.println(" Fan is off");
    lcd.clear();  
    lcd.setCursor(0,0);
    lcd.print("Person In Room: ");
    lcd.setCursor(0,1);
    lcd.print(count);
    p_count = 1;
  }
  else{
    digitalWrite(fan, LOW);
    digitalWrite(light, LOW);
    Serial.print("Person In Room: ");
    Serial.print(count);
    Serial.println(" Fan is on");
    lcd.clear();  
    lcd.setCursor(0,0);
    lcd.print("Person In Room: ");
    lcd.setCursor(0,1);
    lcd.print(count);
    p_count = 1;
  }
  //delay(500);
}
