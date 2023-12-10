void setup() {
  Serial.begin(115200);
  pinMode(40, INPUT);
  pinMode(41, INPUT);
}

void loop() {
  if((digitalRead(41)==1)||(digitalRead(40)==1))
  {
    Serial.println("%");
  }
  else
  {
    Serial.println(analogRead(A0));
  }
  delay(100);
}


