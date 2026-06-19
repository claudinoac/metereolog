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
    while(WiFi.status() != WL_CONNECTED) {
        Serial.print(WiFi.status());
        delay(500);
        Serial.print(".");
    }
    randomSeed(micros());
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("Network IP address: ");
    Serial.println(WiFi.localIP());
}

Wifi::Wifi(char *ssid, char* password, char *ap_ssid, char *ap_pwd) {
    this->ssid = strdup(ssid);
    this->password = strdup(password);
    this->client = new WiFiClientSecure();
    WiFi.begin(this->ssid, this->password);
    WiFi.mode(WIFI_AP_STA);
    Serial.print("Connecting to wifi ");
    Serial.print("'");
    Serial.print(this->ssid);
    Serial.print("'");
    int count = 10;
    while(WiFi.status() != WL_CONNECTED && count > 0) {
        Serial.print(WiFi.status());
        delay(500);
        Serial.print(".");
        count --;
    }
    randomSeed(micros());
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("");
        Serial.println("WiFi connected");
        Serial.print("Network IP address: ");
        Serial.println(WiFi.localIP());
    }

    WiFi.softAP(ap_ssid, ap_pwd);
    Serial.println("AP Started: " + String(ap_ssid));
    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());
}

WiFiClientSecure *Wifi::getClient() {
    return this->client;
}
