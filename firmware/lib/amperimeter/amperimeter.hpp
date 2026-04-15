/*
 * =====================================================================================
 *
 *       Filename:  amperimeter.hpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  26/10/2025 19:28:25
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef AMPERIMETER_HPP
#define AMPERIMETER_HPP

#include <Arduino.h>
#include <driver/gpio.h>

typedef struct {
    float current;
    float measured_voltage;
    float raw_voltage;
} AmperimeterInfo;


class Amperimeter {
    public:
        Amperimeter(gpio_num_t pin, float upper_resistance, float lower_resistance, float conversion_rate = 0.185, float base_voltage = 2.4);
        AmperimeterInfo read();
    private:
        gpio_num_t pin;
        float upper_resistance;
        float lower_resistance;
        float conversion_rate;
        float base_voltage;
        const float voltage_offset = 0.13;
        const float adc_resolution = 4095.0;
        const float ref_voltage = 3.3;
};

#endif
