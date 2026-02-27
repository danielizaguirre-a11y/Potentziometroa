#include <ESP8266WiFi.h>
#include <PubSubClient.h>


// WiFi konfigurazioa
const char* ssid = "Taldea_2"; //Zuen Wifia
const char* password = "OLPtaldea.2"; //Zuen Wifiaren pasahitza


// MQTT konfigurazioa
const char* mqtt_server = "192.168.1.160";  //Zuen ioMBianaren IP helbidea
const int mqtt_port = 1883;
const char* mqtt_user = "iompi";
const char* mqtt_pass = "iompi";
const char* topic = "2";  //Zuen nodMCU-a. Gogoratu 3 dituzuela


// Objektuak
WiFiClient espClient;
PubSubClient client(espClient);


// Pinak
const int flashButton = 0;          // FLASH botoia (GPIO0)
const int led = LED_BUILTIN;        // LED integratua (GPIO2 normalean)


// WiFi konexioa
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("WiFi-ra konektatzen: ");
  Serial.println(ssid);


  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }


  Serial.println("");
  Serial.println("WiFira konektatua!");
  Serial.print("IP helbidea: ");
  Serial.println(WiFi.localIP());
}


// MQTT konexioa
void reconnect() {
  while (!client.connected()) {
    Serial.print("MQTT zerbitzarira konektatzen...");
    if (client.connect("NodeMCUClient2", mqtt_user, mqtt_pass)) {
      Serial.println("konektatua!");
    } else {
      Serial.print("Huts egin du, rc=");
      Serial.print(client.state());
      Serial.println(" 5 segundo barru berriz saiatuko da...");
      delay(5000);
    }
  }
}


void setup() {
  Serial.begin(9600);
  pinMode(flashButton, INPUT_PULLUP);
  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);  // LED itzalita (aktibo baxua da)
 
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}


void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();


  // FLASH botoia sakatuta dagoen detektatu (LOW denean)
  if (digitalRead(flashButton) == LOW) {
    Serial.println("FLASH botoia sakatuta! Mezua bidaltzen...");


    // TRUE mezua bidali
    client.publish(topic, "TRUE");


    // LED piztu mezua bidaltzean (aktibo baxua!)
    digitalWrite(led, LOW);
    digitalWrite(led, HIGH);


    // botoia askatu arte itxaron
    while (digitalRead(flashButton) == LOW);
  }
}
