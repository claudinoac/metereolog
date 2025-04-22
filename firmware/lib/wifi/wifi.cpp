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
    this->client = new WiFiClient();
    WiFi.begin(this->ssid, this->password);
    Serial.print("Connecting to wifi...");
    while(WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    randomSeed(micros());
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

WiFiClient *Wifi::getClient() {
    return this->client;
}
