/*
 * =====================================================================================
 *
 *       Filename:  rtc.cpp
 *
 *    Description: 
 *
 *
 *        Version:  1.0
 *        Created:  27/04/2026 11:41:27
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "rtc.hpp"

RTC::RTC(TwoWire *i2c_channel) {
    this->i2c_channel = i2c_channel;
    this->rtc_clock = new RTC_DS1307();
    int countdown = 10;
    while (!this->rtc_clock->begin(i2c_channel)) {
        Serial.println("Error while configuring RTC module");
        delay(500);
        countdown -= 1;
        if (countdown < 1) {
            throw std::runtime_error("RTC not configured.");
        }
    }
    DateTime current_time = this->rtc_clock->now();
    time_t timestamp;
    if (!this->rtc_clock->isrunning() || !current_time.isValid() || current_time.year() < 2026) {
        Serial.println("Configuring RTC...");
        time_t ntp_timestamp = this->get_ntp_time();
        if (ntp_timestamp > 0) {
            this->rtc_clock->adjust(ntp_timestamp);
            delay(1000);
            Serial.print("Post-adjust: ");
            Serial.println(this->rtc_clock->isrunning() ? "YES" : "NO");
        }
    }
    timestamp = this->rtc_clock->now().unixtime();
    struct tm *t = localtime(&timestamp);
    char buf[20];
    strftime(buf, sizeof(buf), "%d/%m/%y %H:%M", t);
    Serial.print("\n\nTime now is: ");
    Serial.println(buf);
    Serial.println("RTC Configured!");
};

time_t RTC::get_ntp_time() {
    time_t now;
    struct tm timeinfo;
    configTime(0, 0, this->ntp_server);
    while(!getLocalTime(&timeinfo)) {
        Serial.println("Failed to obtain time");
        delay(300);
    }
    time(&now);
    return now;
};

time_t RTC::get_time() {
    return this->rtc_clock->now().unixtime();
}


String RTC::get_time_formatted() {
    time_t timestamp = this->get_time();
    struct tm *t = localtime(&timestamp);
    char buf[20];
    strftime(buf, sizeof(buf), "%d/%m/%y %H:%M", t);
    return String(buf);
}

