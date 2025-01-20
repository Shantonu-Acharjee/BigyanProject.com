
#include <DS3231.h>

int Relay = 4;

DS3231  rtc(SDA, SCL);
Time t;

const int OnHour = 17; //SET TIME TO ON RELAY (24 HOUR FORMAT)
const int OnMin = 40;
const int OffHour = 6; //SET TIME TO OFF RELAY
const int OffMin = 0;

void setup() {
  Serial.begin(115200);
  rtc.begin();
  pinMode(Relay, OUTPUT);
  digitalWrite(Relay, HIGH);
}

void loop() {
  t = rtc.getTime();
  Serial.print(t.hour);
  Serial.print(" hour(s), ");
  Serial.print(t.min);
  Serial.print(" minute(s)");
  Serial.println(" ");
  delay (1000);
  int current_Mins = t.hour*60+t.min;
  int off_Mins = OffHour*60+OffMin;
  int on_Mins = OnHour*60+OnMin;
  Serial.println(current_Mins);
  Serial.println(off_Mins);
  Serial.println(on_Mins);
  if((current_Mins >= off_Mins) && (current_Mins <= on_Mins)){
      digitalWrite(Relay,HIGH);
      Serial.println("LIGHT OFF");
  }    
  else{
      digitalWrite(Relay,LOW);
      Serial.println("LIGHT ON");
  }
}
