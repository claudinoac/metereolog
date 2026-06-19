#ifndef RTC_HPP
#define RTC_HPP

#include <driver/gpio.h>
#include <Arduino.h>
#include <Wire.h>
#include "time.h"
#include "RTClib.h"

class RTC {
    public:
        RTC(TwoWire *i2c_channel);
        String get_time_formatted();
        time_t get_time();
    private:
        TwoWire *i2c_channel;
        RTC_DS1307 *rtc_clock; 
        time_t  get_ntp_time(); 
        const char *ntp_server = "pool.ntp.org";
};

#endif
