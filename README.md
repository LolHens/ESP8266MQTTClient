# MQTT Client library for ESP8266 Arduino
[![Test Workflow](https://github.com/LolHens/ESP8266MQTTClient/workflows/test/badge.svg)](https://github.com/LolHens/ESP8266MQTTClient/actions?query=workflow%3Atest)
[![Release Notes](https://img.shields.io/github/release/LolHens/ESP8266MQTTClient.svg?maxAge=3600)](https://github.com/LolHens/ESP8266MQTTClient/releases/latest)
[![PlatformIO](https://img.shields.io/badge/PlatformIO-ESP8266MQTTClient-%23EE6D0B)](https://platformio.org/lib/show/7453/ESP8266MQTTClient)
[![Apache License 2.0](https://img.shields.io/github/license/LolHens/ESP8266MQTTClient.svg?maxAge=3600)](https://www.apache.org/licenses/LICENSE-2.0)

This is a fork of [tuanpmt/ESP8266MQTTClient](https://github.com/tuanpmt/ESP8266MQTTClient).

This is an MQTT client library for ESP8266, using mqtt_msg package from [MQTT client library for Contiki](https://github.com/esar/contiki-mqtt) for use with the ESP8266 NON-OS SDK [esp_mqtt](https://github.com/tuanpmt/esp_mqtt).

Features:

- Supports subscribing, publishing, authentication, will messages, keep alive pings and all 3 QoS levels (it should be a fully functional client).

## Requirements
- ESP8266WiFi
- WiFiClientSecure

## Status
- Supports all 3 qos types (0, 1, 2) and outbox
- Supports MQTT over TCP, TLS, Websocket and Secure Websocket

## MQTT URI Scheme
- `mqtt://hostname[:port]`
    + `mqtt` for MQTT over TCP
    + `mqtts` for MQTT over TLS
    + `ws` for MQTT over Websocket
    + `wss` for MQTT over Secure Websocket
    + port is 1883 by default
    + client_id is `"ESP_" + ESP.getChipId()` by default
- Example:
    + **Full** `mqtt://test.mosquitto.org:1883`
    + **Websocket** `ws://test.mosquitto.org:1883/mqtt`
    + **Minimal** `mqtt://test.mosquitto.org` (port = 1883)

## API 
### Setup
- bool begin(String uri);
- bool begin(String uri, String username, String password);
- bool begin(String uri, String client_id);
- bool begin(String uri, String client_id, String username, String password);
- bool begin(String uri, int keepalive, bool clean_session);
- bool begin(String uri, String username, String password, int keepalive, bool clean_session);
- bool begin(String uri, String client_id, int keepalive, bool clean_session);
- bool begin(String uri, String client_id, String username, String password, int keepalive, bool clean_session);
- bool begin(String uri, LwtOptions lwt);
- bool begin(String uri, String username, String password, LwtOptions lwt);
- bool begin(String uri, String client_id, LwtOptions lwt);
- bool begin(String uri, String client_id, String username, String password, LwtOptions lwt);
- bool begin(String uri, LwtOptions lwt, int keepalive, bool clean_session);
- bool begin(String uri, String username, String password, LwtOptions lwt, int keepalive, bool clean_session);
- bool begin(String uri, String client_id, LwtOptions lwt, int keepalive, bool clean_session);
- bool begin(String uri, String client_id, String username, String password, LwtOptions lwt, int keepalive, bool clean_session);

### Events
- void onConnect(THandlerFunction fn);
- void onDisconnect(THandlerFunction fn);
- void onSubscribe(THandlerFunction_PubSub fn);
- void onPublish(THandlerFunction_PubSub fn);
- void onData(THandlerFunction_Data fn);

### Pub/Sub
- int subscribe(String topic);
- int subscribe(String topic, uint8_t qos);
- int publish(String topic, String data);
- int publish(String topic, String data, int qos, int retain);

## License
Copyright (c) 2016 Tuan PM (https://twitter.com/tuanpmt)

ESP8266 port (c) 2016 Ivan Grokhotkov (ivan@esp8266.com)

This project uses the Apache 2.0 License. See the file called LICENSE.
