/********************************
 * Programa: NOMBRE
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
 * Fecha de desarrollo: MES / AÑO
 * 
 * Licencia: THE BEER-WARE LICENSE
 * As long as you retain this notice you can do whatever you want with this stuff. 
 * If we meet some day, and you think this stuff is worth it, you can buy me a beer in return.
********************************/
#include <WiFi.h> // Con el ESP8266: #include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#define DHT_PIN 4
#define DHT_TIPO DHT11

DHT dht(DHT_PIN,DHT_TIPO);

WiFiClient espClient; 
PubSubClient client(espClient);

char msg[16];

void setup(){

  dht.begin();
  setup_wifi();
  client.setServer("54.86.86.52",1883);

  pinMode(2,OUTPUT);

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

  snprintf(msg,16,"%d,%d",temperatura,humedad);

  client.publish("canal",msg);
  delay(1000);
}

void reconnect(){

  if(client.connect("ESP32")){
    
    Serial.println(".......Conexión exitosa");
  }
}

void setup_wifi(){

  Serial.begin(9600);
  WiFi.begin("Proyectoprocesos2","Uam11111"); //se habilitará el modo estación

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

}
