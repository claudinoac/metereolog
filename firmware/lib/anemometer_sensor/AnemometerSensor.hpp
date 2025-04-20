
#ifndef ANEMOMETERSENSOR_H
#define ANEMOMETERSENSOR_H

#include <driver/gpio.h>
#include <Arduino.h>

typedef struct {
    float wind_speed;    // km/h
    unsigned int pulses; // pulse count
} AnemometerInfo;

class AnemometerSensor {
    public:
        AnemometerSensor(gpio_num_t pin);
        AnemometerInfo read();
        void IRAM_ATTR handleInterrupt();
        static AnemometerSensor *getInstance();
        
    private:
        gpio_num_t pin;
        volatile unsigned int pulseCount;
        unsigned long ts_read;
};

#endif
