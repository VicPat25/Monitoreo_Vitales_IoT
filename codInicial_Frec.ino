int potenciometro = 36;
int valorPot = 0;
void setup() {
  Serial.begin(115200);
}

void loop() {
  valorPot = analogRead(potenciometro);
  Serial.println(valorPot);
  delay(800);

  
  
}
