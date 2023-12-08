#include <OneWire.h>
#include <DallasTemperature.h>

const int oneWireBus = 5;
int datoECG;

OneWire oneWire(oneWireBus);

DallasTemperature sensors (&oneWire);

void setup() {
  Serial.begin(115200);
  sensors.begin();
  pinMode(40, INPUT);
  pinMode(41, INPUT);
}

void loop() {
  Serial.print("Temperatura del paciente ");
  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);
  Serial.print(temperatureC);
  Serial.println("°C");
  datoECG=tomarECG();
  delay(100);
}

int tomarECG (){
  if((digitalRead(41)==1)||(digitalRead(40)==1))
  {
    Serial.println("%");
  }
  else
  {
    int ECG=analogRead(A0);
    return ECG;
    Serial.println(analogRead(A0));
  }
}