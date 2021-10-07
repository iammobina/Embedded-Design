void setup() 
{
  pinMode(8, OUTPUT);
}
 
void loop() {
  digitalWrite(8,LOW);
  delay(1000);              // wait for 5 seconds
  digitalWrite(8,HIGH);
  delay(500);              // wait for 5 seconds
}
