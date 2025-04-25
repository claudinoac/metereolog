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
    Wifi *wifi_client, char *broker_addr, int broker_port, char *username, char *password, char *client_id
) {
    this->wifi_client = wifi_client;
    this->client = new PubSubClient(*this->wifi_client->getClient());
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
    this->client->setServer(this->broker_addr, this->broker_port);
    this->client->setCallback(this->handle_incoming_message);
};

void MQTT::handle_incoming_message(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived [");
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
        if (client->connect(this->client_id, this->username, this->password)) {
            Serial.println("connected");
            client->subscribe(topic);
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

void MQTT::publish(char *topic, JsonDocument &message) {
    if (!this->client->connected()) {
        this->connect(topic);
    }
    this->client->loop();
    char msg_buffer[MSG_BUFFER_SIZE];
    serializeJson(message, msg_buffer);
    this->client->publish(topic, msg_buffer, MSG_BUFFER_SIZE);
}
