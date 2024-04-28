void setup() {
  pinMode(7, INPUT);
  pinMode(9, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(9, LOW);
  Serial.print("High ");
  Serial.println(digitalRead(7));
  delay(1000);
  
  digitalWrite(9, HIGH);
  Serial.print("Low ");
  Serial.println(digitalRead(7));
  delay(1000);
  
}
