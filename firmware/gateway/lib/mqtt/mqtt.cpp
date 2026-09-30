/*
 * =====================================================================================
 *
 *       Filename:  mqtt.cpp
 *
 *    Description: Wrapper to manage MQTT Connection
 *
 *        Version:  0.0.1
 *        Created:  22/04/2025 05:19:56
 *       Revision:  none
 *       Compiler:  clang
 *
 *         Author:  Alisson Claudino
 *   Organization:  UFSC
 *
 * =====================================================================================
 */
#include "mqtt.hpp"


MQTT::MQTT(
    Wifi *wifi_client, char *broker_addr, int broker_port, char *username, char *password) {
    this->wifi_client = wifi_client;
    this->client = new PubSubClient(*this->wifi_client->getClient());
    if(!this->client->setBufferSize(MQTT_MAX_PACKET_SIZE)) {
        Serial.print("Cannot allocate buffer of size");
        Serial.print(MQTT_MAX_PACKET_SIZE);
        Serial.println(" to MQTT device");
    };
    this->username = strdup(username);
    this->password = strdup(password);
    this->broker_addr = strdup(broker_addr);
    this->broker_port = broker_port;
    this->password = strdup(password);
    if (this->client_id == NULL) {
        this->client_id = (char *)("ESP32" + String(random((0xffff), HEX))).c_str();
    } else {
        this->client_id = strdup(client_id);
    }
    this->wifi_client->getClient()->setInsecure();
    this->client->setServer(this->broker_addr, this->broker_port);
    this->client->setCallback(this->handle_incoming_message);
};

void MQTT::handle_incoming_message(char* topic, byte* payload, unsigned int length) {
    Serial.print("\n\nMessage arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
    }
    Serial.println();
};

void MQTT::connect(char *topic) {
    // Loop until we're reconnected
    while (!this->client->connected()) {
        Serial.print("Attempting MQTT connection...");
        if (this->client->connect(this->client_id, this->username, this->password)) {
            Serial.println("connected");
            this->client->setKeepAlive(30);
            this->client->setSocketTimeout(15);
            // client->subscribe(topic);
        } else {
            Serial.print("failed, rc=");
            Serial.print(this->client->state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void MQTT::loop() {
    this->client->loop();
}

void MQTT::publish(char *topic, String message) {
    if (!this->client->connected()) {
        this->connect(topic);
    }
    message.toCharArray(this->msg_buffer, MSG_BUFFER_SIZE);
    if(this->client->publish(topic, this->msg_buffer, strlen(this->msg_buffer))) {
        Serial.print("\n\n Sent ");
        Serial.print(strlen(this->msg_buffer));
        Serial.print(" bytes to topic ");
        Serial.print(topic);
        Serial.print("\n\n");
    } else {
        Serial.print("Publish to topic ");
        Serial.print(topic);
        Serial.print(" failed! ");
        int payload_size = strlen(this->msg_buffer);
        int topic_size = strlen(topic);
        Serial.printf(
          "payload=%u | topic=%u | packet~=%u | buffer=%u | state=%d | wifi=%d | heap=%u\n\n",
          payload_size,
          topic_size,
          payload_size + topic_size + 7,
          this->client->getBufferSize(),
          this->wifi_client->getClient()->connected(),
          ESP.getFreeHeap()
        );

        Serial.print("Reason: ");
        Serial.print(this->client->state());
    }
    this->client->loop();
}
