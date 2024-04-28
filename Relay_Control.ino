void setup() {
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(7, OUTPUT);
  
  play_Siren(0, 0);
}

void loop() {
  play_Siren(0, 0); //Turn off all sirens
  play_Siren(1, 5000); //Play siren 1 sound for 5 seconds
  play_Siren(2, 10000); //Play siren 2 sound for 10 seconds
  play_Siren(3, 10000); //Play siren 3 sound for 10 seconds
  
  
}

void play_Siren(int severity, int length) {

  switch (severity){

    case 0:
      digitalWrite(9, LOW);
      digitalWrite(8, LOW);
      digitalWrite(7, LOW);
    break;

    case 1:
      digitalWrite(9, HIGH);
      delay(length);
      digitalWrite(9, LOW);
    break;

    case 2:
      digitalWrite(8, HIGH);
      digitalWrite(8, LOW);
      delay(length);
      digitalWrite(8, HIGH);
      digitalWrite(8, LOW);
    break;

    case 3:
      digitalWrite(7, HIGH);
      digitalWrite(7, LOW);
      delay(length);
      digitalWrite(7, HIGH);
      digitalWrite(7, LOW);
    break;
  }

}
