/********************************
 * Programa: NOMBRE
 * 
 * Descripción: En este código se realiza una comunicación MQTT en una Red LAN mediante una esp32 y NodeRed 
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
#include <WiFi.h>
#include <PubSubClient.h>
#define BUILTIN_LED 2



const char* ssid = "Educatronicos";
const char* password = "Edu12345";
const char* mqtt_server = "broker.emqx.io";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {

  delay(10);
 
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


  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   
   
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  
    
  }

}

void reconnect() {
  
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      
      client.publish("outTopic", "hello world");
      
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
  pinMode(BUILTIN_LED, OUTPUT);     
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    value = random(1,100);
    snprintf (msg, MSG_BUFFER_SIZE, "hello world #%ld", value);
    Serial.print("Publish message: ");
    
    Serial.println(msg);
    client.publish("outTopic", msg);
  }
}
