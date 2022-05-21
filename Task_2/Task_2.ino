#include <LiquidCrystal_I2C.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <WiFi.h>

#define SDA 13 //Define SDA pins
#define SCL 14 //Define SCL pins

/*
* note: If lcd1602 uses PCF8574T, IIC's address is 0x27,
* or lcd1602 uses PCF8574AT, IIC's address is 0x3F.
*/
LiquidCrystal_I2C lcd(0x27,16,2); 

//WiFi
const char *ssid = "SpectrumSetup-F0"; // Enter your WiFi name
const char *password = "mahalkita825";  // Enter WiFi password

// MQTT Broker
const char *mqtt_broker = "192.168.56.1";
const char *topic = "esp32/test";
//const char *mqtt_username = "emqx";
//const char *mqtt_password = "123456789";
const int mqtt_port = 1883;

int randNum;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {

  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
   //connecting to a mqtt broker
  client.setServer(mqtt_broker, mqtt_port);
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
 Wire.begin(SDA, SCL); // attach the IIC pin
 lcd.init(); // LCD driver initialization
 lcd.backlight(); // Turn on the backlight
 lcd.setCursor(0,0); // Move the cursor to row 0, column 0
 lcd.print("Welcome Magic 8ball");
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("1. Play");
 lcd.setCursor(0,1); 
 lcd.print("2. Refresh");
 lcd.clear();
 for (int i = 0; i < length; i++) {
     Serial.print((char) payload[i]);
     
     if((char)payload[i] == '1'){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Choice 1 selected");
      lcd.setCursor(0,1);
      lcd.print("Ask a Question");
      delay(3000);
      lcd.clear();
      randNum = random(1,4);

      lcd.setCursor(0,0);
      lcd.print("Thinking");
      delay(1000);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Answer:");

      if(randNum == 1){
        lcd.setCursor(0,1);
        lcd.print("Yes");
      }else if(randNum == 2){
        lcd.setCursor(0,1);
        lcd.print("No");
      }else if(randNum == 3){
        lcd.setCursor(0,1);
        lcd.print("Maybe");
      }else{
        lcd.setCursor(0,1);
        lcd.print("Try again later");
      }
      
     }else if ((char)payload[i] == '2'){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Choice 2 selected");
      lcd.setCursor(0,1);
      lcd.print("Refreshing");
      delay(1500);
      lcd.clear();
     }
 }
 Serial.println();
 Serial.println("-----------------------");
}

void loop() {
  
  client.loop();

}
