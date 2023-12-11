/*******************************************************************************************
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
***********************************************************************************************/

#include <WiFi.h> // Con el ESP8266: #include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
const int oneWireBus = 5;
int datoECG;
int datoAnterior = 0;
int datoActual = 0;
int limite = 1690;
unsigned long tiempo_1 = 0;
unsigned long tiempo_2 = 0;
unsigned long tiempoTotal = 0;

// Update these with values suitable for your network.

const char* ssid = "JuanMCD";
const char* password = "1234567890";
const char* mqtt_server = "broker.emqx.io";


OneWire oneWire(oneWireBus);

DallasTemperature sensors (&oneWire);
WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;
bool band = false;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  pinMode(40, INPUT);
  pinMode(41, INPUT);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    int hr = tomarECG();
    sensors.requestTemperatures();
    float temperatureC = sensors.getTempCByIndex(0);
    String pulse = String(hr);
    String temp = String(temperatureC);
    client.publish("/pulso", pulse.c_str());
    client.publish("/temp", temp.c_str());
  }
}


 int tomarECG (){
  if((digitalRead(41)==1)||(digitalRead(40)==1))
  {
    Serial.println("%");
  }
  else
  {
    int ECG=analogRead(A0);
    datoActual = ECG;
    Serial.println(analogRead(A0));
   
    if((datoActual>limite && datoAnterior<limite)&& (band == false)){
      tiempo_1 = millis();
      band = true;
    }else if((datoActual>limite && datoAnterior<limite)&&(band==true)){
      tiempo_2 = millis();
      tiempoTotal = tiempo_1 - tiempo_2;
      int pulso = 12000/tiempoTotal;
      return pulso;
    }
    datoAnterior = datoActual;
  }

}
