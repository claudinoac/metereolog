/*
 * =====================================================================================
 *
 *       Filename:  mqtt.hpp
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
#ifndef MQTT_H
#define MQTT_H

#include <Arduino.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "wifi.hpp"


#define MSG_BUFFER_SIZE  256

class MQTT {
    PubSubClient *client;
    Wifi *wifi_client;
    char msg[MSG_BUFFER_SIZE];
    char *username;
    char *password;
    char *broker_addr;
    int broker_port;
    char *client_id;

    public:
        MQTT(
            Wifi *wifi_client, char *broker_addr, int broker_port,
            char *username, char *password, char *client_id = NULL
        );

        void connect(char *topic);
        void publish(char *topic, JsonDocument &message);
    private:
        void loop();
        static void handle_incoming_message(char* topic, byte* payload, unsigned int length);
};
#endif
