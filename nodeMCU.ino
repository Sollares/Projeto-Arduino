#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Configurações de Wi-Fi e MQTT
const char* ssid = "";
const char* password = "";
const char* mqtt_server = "";
const char* mqtt_topic = "";

WiFiClient espClient;
PubSubClient client(espClient);

// Pino do LDR
const int ldrPin = A0;  // Entrada analógica para o sensor de luz

// Função para conectar ao Wi-Fi
void setupWiFi() {
  delay(10);
  Serial.println("Conectando ao Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado ao Wi-Fi!");
}

// Reconexão ao MQTT
void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentando conectar ao MQTT...");
    if (client.connect("ESP8266Client")) {
      Serial.println("Conectado!");
    } else {
      Serial.print("Falha. Tentando novamente em 5 segundos...");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setupWiFi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Leitura do sensor LDR
  int lightValue = analogRead(ldrPin);
  Serial.print("Nível de luz: ");
  Serial.println(lightValue);

  // Publica o valor do sensor no MQTT
  String lightStr = String(lightValue);
  client.publish(mqtt_topic, lightStr.c_str());

  delay(2000);  // Aguarda 2 segundos antes de ler novamente
}
