#include <ESP8266WiFi.h>

#include <PubSubClient.h>

const char* ssid = "";
const char* password = "";

const char* mqtt_server = "test.mosquitto.org"; // IP ou URL do broker
const int mqtt_port = 1883;
const char* mqtt_topic = "casa/sensor/luz";

WiFiClient espClient;
PubSubClient client(espClient);

const int ldrPin = A0;

void setup_wifi() {
  delay(10);
  Serial.println("Conectando ao WiFi...");
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi conectado!");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando ao MQTT...");
    if (client.connect("ESP8266Client")) { 
      Serial.println("Conectado!");
    } else {
      Serial.print("Falha, rc=");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 5 segundos...");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  int ldrValue = analogRead(ldrPin);

  String payload = String(ldrValue);
  client.publish(mqtt_topic, payload.c_str());
  Serial.println("Publicado no MQTT: " + payload);

  delay(2000);
}
