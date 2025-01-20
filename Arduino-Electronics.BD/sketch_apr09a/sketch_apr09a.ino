int Object = 0;
void setup()
{ 
 
 // Debug console
 Serial.begin(115200);
 Serial1.begin(115200);
 pinMode(2,OUTPUT);

}
 
void loop()
{
 digitalWrite(2,HIGH);
 while (Serial1.available()>0)  {
    Object = Serial1.parseInt();
    if(Object == 1002){
      Serial.println("found");
    }
    else{
      Serial.println(Object);
    }
 }
 
}
