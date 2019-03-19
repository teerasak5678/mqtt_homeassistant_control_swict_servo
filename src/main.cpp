#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_Sensor.h>
#include <Servo.h>

const char* ssid = "Inspiration";
const char* password =  "inspiration1";
const char* mqttServer = "mqtt.ppsmartbot.com";
const int mqttPort = 1883;
//const char* mqttUser = &quot;YourMqttUser&quot;;
//const char* mqttPassword = &quot;YourMqttUserPassword&quot;;
Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards
int pos;

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

       client.publish("homeassistant/esp/test","online");
       pinMode(motor, OUTPUT);
       digitalWrite(motor,LOW);

     }
     if (strPayload == "offline")
     {
       client.publish("homeassistant/esp/test","offline");
      pinMode(motor, OUTPUT);
      digitalWrite(motor,HIGH);


     }

     if (strPayload == "dooron")
     {
       client.publish("homeassistant/esp/test","dooron");
       myservo.attach(5);  // attaches the servo on GIO2 to the servo object
       for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
         // in steps of 1 degree
         myservo.write(pos);              // tell servo to go to position in variable 'pos'
       // waits 15ms for the servo to reach the position
       }
      }
      if (strPayload == "dooroff")
      {
        client.publish("homeassistant/esp/test","dooroff");
       myservo.attach(5);  // attaches the servo on GIO2 to the servo object
       for (pos = 90; pos >= 0; pos -= 1) { // goes from 0 degrees to 180 degrees
         // in steps of 1 degree
         myservo.write(pos);              // tell servo to go to position in variable 'pos'
                    // waits 15ms for the servo to reach the position
       }
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
