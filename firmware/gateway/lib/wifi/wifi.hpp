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

#ifndef WIFI_H
#define WIFI_H

#include <WiFi.h>
#include <WiFiClientSecure.h>


class Wifi {
    WiFiClientSecure *client;
    char *ssid;
    char *password;
    String ip;

    public:
        Wifi(char *ssid, char *password);
        WiFiClientSecure *getClient();
        String get_ip();
};

#endif
