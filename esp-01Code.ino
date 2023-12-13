#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

// Substitua com suas credenciais Adafruit IO
#define IO_USERNAME "NOME DO USUARIO NO ADAFRUIT"
#define IO_KEY "IO_KEY DO ADAFRUIT"
const char* WIFI_SSID = "NOME DO WIFI"; 
const char* WIFI_PASS = "SENHA DO WIFI";

// Nome dos feeds no Adafruit IO
#define UMIDADE_DO_SOLO_FEED "umidade-do-solo"
#define TEMPERATURA_FEED "temperatura"
#define LUMINOSIDADE_FEED "luminosidade"
#define ULTRASSONICO_FEED "sensor-ultrassonico"
#define RESERVATORIO_FEED "reservatorio"
#define TEMPO_DE_ESPERA 30000
long long ultimaLeitura = 0;

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, "io.adafruit.com", 1883, IO_USERNAME, IO_KEY);

Adafruit_MQTT_Publish umidadePublish = Adafruit_MQTT_Publish(&mqtt, IO_USERNAME "/feeds/" UMIDADE_DO_SOLO_FEED);
Adafruit_MQTT_Publish temperaturaPublish = Adafruit_MQTT_Publish(&mqtt, IO_USERNAME "/feeds/" TEMPERATURA_FEED);
Adafruit_MQTT_Publish luminosidadePublish = Adafruit_MQTT_Publish(&mqtt, IO_USERNAME "/feeds/" LUMINOSIDADE_FEED);
Adafruit_MQTT_Publish ultrassonicoPublish = Adafruit_MQTT_Publish(&mqtt, IO_USERNAME "/feeds/" ULTRASSONICO_FEED);
Adafruit_MQTT_Publish reservatorioPublish = Adafruit_MQTT_Publish(&mqtt, IO_USERNAME "/feeds/" RESERVATORIO_FEED);

void setup() {
  Serial.begin(115200);
  delay(10);

  // Conecte-se à rede Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }
  Serial.println("Conectado ao WiFi");

  connectToAdafruitIO();
}

void loop() {
  if (Serial.available() > 0) {
    // Leia os valores enviados pelo Arduino via Serial
    String data = Serial.readStringUntil('\n');
    float umidadeDoSolo, temperatura, luminosidade, espaco, reservatorio;
       int parsed = sscanf(data.c_str(), "%f %f %f %f %f", &umidadeDoSolo, &temperatura, &luminosidade, &espaco, &reservatorio);

    if (parsed == 5 && mqtt.connected()){
      if((millis()- ultimaLeitura ) > TEMPO_DE_ESPERA){
        umidadePublish.publish(umidadeDoSolo);
        temperaturaPublish.publish(temperatura);
        luminosidadePublish.publish(luminosidade);
        ultrassonicoPublish.publish(espaco);
        reservatorioPublish.publish(reservatorio);
        // Dados publicados para o Adafruit IO
        Serial.println("Dados enviados para o Adafruit IO");
        ultimaLeitura = millis();
      }
    }
  }
  //PREVENÇÃO CONTRA DESCONEXÕES
  if (WiFi.status() != WL_CONNECTED) {
    reconnectWiFi();
  }
  if (!mqtt.connected()) {
    reconnectMQTT();
  }
}

void connectToAdafruitIO() {
  Serial.println("Conectando ao Adafruit IO...");
  int8_t ret;
  while ((ret = mqtt.connect()) != 0) {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Tentando reconectar...");
    delay(3000);
  }
  Serial.println("Conectado ao Adafruit IO!");
}

void reconnectMQTT() {
  Serial.println("Conexão MQTT perdida. Tentando reconectar...");
  connectToAdafruitIO(); // Função para conectar ao Adafruit IO
}

void reconnectWiFi() {
  Serial.println("Conexão WiFi perdida. Tentando reconectar...");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }
  Serial.println("Conectado ao WiFi");
}
