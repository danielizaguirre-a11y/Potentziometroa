#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// -------- KONFIGURAZIOA --------
const char* ssid = "OLP";
const char* password = "oteitzaLP";

const char* mqtt_server = "eu.thingsboard.cloud";
const char* token = "JRkVQ2gaZsEHFYB9OHiv";

#define POT_PIN A0

WiFiClient espClient;
PubSubClient client(espClient);

// -------- WIFI KONEXIOA --------
void wifi_konektatu() {
  delay(10);
  Serial.println();
  Serial.print("WiFira konektatzen: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi konektatuta!");
}

// -------- MQTT BERRIZ KONEXIOA --------
void berriz_konektatu() {
  while (!client.connected()) {
    Serial.print("ThingsBoard-era konektatzen...");
    if (client.connect("NodeMCU", token, NULL)) {
      Serial.println("Konektatuta!");
    } else {
      Serial.print("Errorea rc=");
      Serial.print(client.state());
      Serial.println(" 5 segundotan berriz saiatzen...");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  wifi_konektatu();
  client.setServer(mqtt_server, 1883);
}

void loop() {

  if (!client.connected()) {
    berriz_konektatu();
  }
  client.loop();

  // Potentziometroaren balioa irakurri (0–1023)
  int balioGordina = analogRead(POT_PIN);

  // 0–1023 → 0–100 eskalara bihurtu
  int balioMapeatua = map(balioGordina, 0, 1023, 0, 100);

  // Voltaje kalkulua (aukerakoa)
  float tentsioa = balioGordina * (3.3 / 1023.0);

  char payload[120];
  sprintf(payload, "{\"potentziometroa\":%d,\"tentsioa\":%.2f}", balioMapeatua, tentsioa);

  client.publish("v1/devices/me/telemetry", payload);

  Serial.println(payload);

  delay(2000); // 2 segundoro bidali
}
