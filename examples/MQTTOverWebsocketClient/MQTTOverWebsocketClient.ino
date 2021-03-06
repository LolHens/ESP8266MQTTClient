#include <ESP8266MQTTClient.h>
#include <ESP8266WiFi.h>
MQTTClient mqtt;

void setup() {
  Serial.begin(115200);
  //  WiFi.begin("ssid", "pass");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //topic, data, data is continuing
  mqtt.onData([](String topic, String data, bool cont) {
    Serial.printf("Data received, topic: %s, data: %s\r\n", topic.c_str(), data.c_str());
    mqtt.unSubscribe("/qos0");
  });

  mqtt.onSubscribe([](int sub_id) {
    Serial.printf("Subscribe topic id: %d ok\r\n", sub_id);
    mqtt.publish("/qos0", "qos0", 0, 0);
  });
  mqtt.onConnect([]() {
    Serial.printf("MQTT: Connected\r\n");
    Serial.printf("Subscribe id: %d\r\n", mqtt.subscribe("/qos0", 0));
    mqtt.subscribe("/qos1", 1);
    mqtt.subscribe("/qos2", 2);
  });

  mqtt.begin("ws://broker.mqttdashboard.com:8000/mqtt");
  //mqtt.begin("ws://test.mosquitto.org:8080", {.lwtTopic = "hello", .lwtMsg = "offline", .lwtQos = 0, .lwtRetain = 0});
  //mqtt.begin("ws://mosquito.org:8080", "user", "pass");
  //mqtt.begin("ws://mosquito.org:8080", "clientId", "user", "pass");

}

void loop() {
  mqtt.handle();
}
