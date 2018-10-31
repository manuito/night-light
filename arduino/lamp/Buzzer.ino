void startBuzzer(){
  pinMode(BUZZER_PIN, OUTPUT);
}

void doBuzz(){
  digitalWrite(BUZZER_PIN, HIGH);
}

void stopBuzz(){
  digitalWrite(BUZZER_PIN, LOW);
}

