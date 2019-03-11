#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>


const char* ssid = "Inspiration";
const char* password =  "inspiration1";
const char* mqttServer = "mqtt.ppsmartbot.com";
const int mqttPort = 1883;
//const char* mqttUser = &quot;YourMqttUser&quot;;
//const char* mqttPassword = &quot;YourMqttUserPassword&quot;;
//int led1 = D0;
//int led2 = D1;
int motor = D5;

#define DHTTYPE DHT22

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
  char p[length + 1];
     memcpy(p, payload, length);
     p[length] = NULL;
     String message(p);

     payload[length] = '\0';
     String strPayload = String((char*)payload);

     if (strPayload == "online")
     {
       //getTemp();
       client.publish("homeassistant/esp/test","1");
       pinMode(motor, OUTPUT);
       digitalWrite(motor,LOW);
       //pinMode(led1, OUTPUT);
       //digitalWrite(led1,LOW);
       //pinMode(led2, OUTPUT);
       //digitalWrite(led2,LOW);


       //delay(2000);
      // pinMode(online, INPUT);
     }
     if (strPayload == "offline")
     {
       client.publish("homeassistant/esp/test","0");
      pinMode(motor, OUTPUT);
      digitalWrite(motor,HIGH);
      //pinMode(led1, OUTPUT);
      //digitalWrite(led1,HIGH);
      //pinMode(led2, OUTPUT);
      //digitalWrite(led2,HIGH);
      //delay(2000);
      //pinMode(offline, INPUT);
     }
}

void setup() {

  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");

    if (client.connect("ESP8266Client")) {

      Serial.println("connected");

    } else {

      Serial.print("failed with state");
      Serial.print(client.state());
      delay(2000);

    }
  }

  client.publish("homeassistant/esp8266", "Hello from ESP8266");
  client.subscribe("homeassistant/esp/status/set");

}



void loop() {
  client.loop();
}
