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


class Wifi {
    WiFiClient *client;
    char *ssid;
    char *password;

    public:
        Wifi(char *ssid, char *password);
        WiFiClient *getClient();
};

#endif
