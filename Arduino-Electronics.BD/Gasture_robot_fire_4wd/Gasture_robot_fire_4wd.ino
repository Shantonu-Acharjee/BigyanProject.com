#include<Wire.h>
const int MPU=0x68; 
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

void  setup(){
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);  
  Wire.write(0);    
  Wire.endTransmission(true);
  Serial.begin(9600);
}
void  loop(){
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,12,true);  
  AcX=Wire.read()<<8|Wire.read();    
  AcY=Wire.read()<<8|Wire.read();  
  AcZ=Wire.read()<<8|Wire.read();   
  
  Serial.print("Accelerometer: ");
  Serial.print("X = "); Serial.print(AcX);
  Serial.print(" | Y = "); Serial.print(AcY);
  Serial.print(" | Z = ");  Serial.println(AcZ); 
  if (AcX < 1000 && AcY < -8000) {
    Serial.println("UP");
  } else if (AcX < 1000 && AcY > 8000) {
    Serial.println("Down");
  } else if (AcX > 8000 && AcY < 1000) {
    Serial.println("LEFT");
  } else if (AcX < -8000 && AcY < 1000) {
    Serial.println("RIGHT");
  } else {
  }
  
  Serial.println(" ");
  delay(333);
}
