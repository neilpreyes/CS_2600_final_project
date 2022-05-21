#include <WiFi.h>
#include <PubSubClient.h>
#define PIN_LED 2

// WiFi
const char *ssid = "SpectrumSetup-F0"; // Enter your WiFi name
const char *password = "mahalkita825";  // Enter WiFi password

// MQTT Broker
const char *mqtt_broker = "192.168.56.1";
const char *topic = "esp32/test";
//const char *mqtt_username = "emqx";
//const char *mqtt_password = "123456789";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
 // Set software serial baud to 115200;
 Serial.begin(115200);
 // connecting to a WiFi network
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.println("Connecting to WiFi..");
 }
 Serial.println("Connected to the WiFi network");
 //connecting to a mqtt broker
 client.setServer(mqtt_broker, mqtt_port);
 pinMode(PIN_LED, OUTPUT);
 client.setCallback(callback);
 while (!client.connected()) {
     String client_id = "esp32-client-";
     client_id += String(WiFi.macAddress());
     Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
     if (client.connect(client_id.c_str())) {
         Serial.println("Public emqx mqtt broker connected");
     } else {
         Serial.print("failed with state ");
         Serial.print(client.state());
         delay(2000);
     }
 }
 // publish and subscribe
 client.publish(topic, "Hi EMQX I'm ESP32 ^^");
 client.subscribe(topic);
}

void callback(char *topic, byte *payload, unsigned int length) {
 Serial.print("Message arrived in topic: ");
 Serial.println(topic);
 Serial.print("Message:");
 for (int i = 0; i < length; i++) {
     Serial.print((char) payload[i]);
     
     if((char)payload[i] == '1'){
       digitalWrite(PIN_LED,HIGH);
     }else{
       digitalWrite(PIN_LED,LOW);
     }
 }
 Serial.println();
 Serial.println("-----------------------");
}

void loop() {
 client.loop();
}
