#ifndef ANEMOMETER_H
#define ANEMOMETER_H

#include <Arduino.h>
#include <driver/gpio.h>

typedef struct {
    float windSpeed;
    float rotations;
} WindInfo;

class Anemometer {
public:
    Anemometer(gpio_num_t pin, float radius = 14.7);
    WindInfo read();

private:
    gpio_num_t inputPin;
    float radius;
};

#endif
