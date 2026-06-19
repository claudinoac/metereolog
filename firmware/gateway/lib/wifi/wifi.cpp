/*
 * =====================================================================================
 *
 *       Filename:  wifi.hpp
 *
 *    Description: Wrapper library for wifi connection
 *
 *        Version:  1.0
 *        Created:  22/04/2025 04:20:20
 *       Revision:  none
 *       Compiler:  clang
 *
 *         Author:  Alisson Claudino
 *   Organization:  UFSC
 *
 * =====================================================================================
 */
#include "wifi.hpp"

Wifi::Wifi(char *ssid, char* password) {
    this->ssid = strdup(ssid);
    this->password = strdup(password);
    this->client = new WiFiClientSecure();
    WiFi.begin(this->ssid, this->password);
    WiFi.mode(WIFI_STA);
    Serial.print("Connecting to wifi ");
    Serial.print("'");
    Serial.print(this->ssid);
    Serial.print("'");
    int countdown = 20;
    while(WiFi.status() != WL_CONNECTED) {
        Serial.print(WiFi.status());
        delay(500);
        Serial.print(".");
        countdown -= 1;
        if (countdown < 1) {
            throw std::runtime_error("Wifi not configured.");
        }
    }
    randomSeed(micros());
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    this->ip = WiFi.localIP().toString();
}

String Wifi::get_ip() {
    return this->ip;
}

WiFiClientSecure *Wifi::getClient() {
    return this->client;
}
