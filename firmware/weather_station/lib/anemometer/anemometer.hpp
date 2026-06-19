#ifndef ANEMOMETER_HPP
#define ANEMOMETER_HPP

#include <driver/gpio.h>
#include <Arduino.h>

typedef struct {
    float wind_speed;    // km/h
    unsigned int pulses; // pulse count
} AnemometerInfo;

class Anemometer {
    public:
        Anemometer(gpio_num_t pin);
        AnemometerInfo read();
        void IRAM_ATTR handleInterrupt();
        static Anemometer *getInstance();
        
    private:
        gpio_num_t pin;
        volatile unsigned int pulseCount;
        unsigned long ts_read;
};

#endif
