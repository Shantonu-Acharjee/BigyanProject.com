int led1 = 10;
int led2 = 11;
int led3 = 12;
int led4 = 14;
int led5 = 15;
int led6 = 16;
int led7 = 18;
int led8 = 19;
void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.print(analogRead(led1));
  Serial.print("=");
  Serial.print(analogRead(led2));
  Serial.print("=");
  Serial.print(analogRead(led3));
  Serial.print("=");
  Serial.print(analogRead(led4));
  Serial.print("=");
  Serial.print(analogRead(led5));
  Serial.print("=");
  Serial.print(analogRead(led6));
  Serial.print("=");
  Serial.print(analogRead(led7));
  Serial.print("=");
  Serial.print(analogRead(led8));
  Serial.println();

}
