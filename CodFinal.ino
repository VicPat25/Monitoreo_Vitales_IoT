/********************************
 * Programa: Envío de datos de sensor de temperatura y frecuencia cardiaca a Node-RED
 * 
 * Descripción:
 *
 * Director: 
 *   - Juan Manuel Calvo Duque
 * Co-directores:
 *   - David Ricardo Rivera Arbelaez
 *   - Víctor Manuel Patiño Delgado
 * Autores:
 *   - Daniela Moreno Grisales
 *   - Valeria Valencia López. 
 * 
 * Fecha de desarrollo: NOVIEMBRE / 2023
 * 
 * Licencia: THE BEER-WARE LICENSE
 * As long as you retain this notice you can do whatever you want with this stuff. 
 * If we meet some day, and you think this stuff is worth it, you can buy me a beer in return.
********************************/
#include <WiFi.h> // Con el ESP8266: #include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
const int oneWireBus = 5;
int datoECG;

OneWire oneWire(oneWireBus);

DallasTemperature sensors (&oneWire);

WiFiClient espClient; 
PubSubClient client(espClient);

char msg[16];

void setup(){

   Serial.begin(115200);
  sensors.begin();
  
  client.setServer("54.86.86.52",1883);

  pinMode(40, INPUT);
  pinMode(41, INPUT);

  client.setCallback(callback);
}

void loop(){

  if(!client.connected()){

    reconnect();
  }

  client.loop();
  
  //----------------------------------
  delay(2000);
  int temperatura=dht.readTemperature();
  int humedad=dht.readHumidity();
  //----------------------------------

  snprintf(msg,16,"%d,%d",temperatura,frecuencia cardiaca);

  client.publish("canal",msg);
  delay(1000);
  Serial.print("Temperatura del paciente ");
  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);
  Serial.print(temperatureC);
  Serial.println("°C");
  datoECG=tomarECG();
  delay(100);
}

void reconnect(){

  if(client.connect("ESP32")){
    
    Serial.println(".......Conexión exitosa");
  }
}

void setup_wifi(){

  Serial.begin(9600);
  WiFi.begin("FAMILIAMORENO","69782102"); //se habilitará el modo estación

  // Mientras el ESP32 no se conecte al AP:
  while(WiFi.status() != WL_CONNECTED){
    delay(200);
    Serial.print(".");
  }
  
  //Serial.println(WiFi.localIP()); //Imprimo el IP del esp32
}

void callback(char* topic, byte* payload, unsigned int length){

  payload[length]='\0';
  String val = String((char*)payload);


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

}
