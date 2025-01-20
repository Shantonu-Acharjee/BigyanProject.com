#define REPORTING_PERIOD_MS     5000
uint32_t tsLastReport = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(2,INPUT_PULLUP);
  pinMode(3,OUTPUT);
  while(!Serial) {} // Portability for Leonardo/Micro
  
}

void loop()
{
  if(digitalRead(2)){
    digitalWrite(3,LOW);
  }
  else{
    digitalWrite(3,HIGH);
  }
  if (millis() - tsLastReport > REPORTING_PERIOD_MS){
    int PPM1 = analogRead(A0)/10;  
    int PPM2 = analogRead(A1)/10; 
    String str =String(PPM1)+String("=")+String(PPM2);
    Serial.println(str);
    tsLastReport = millis();
  }
}
