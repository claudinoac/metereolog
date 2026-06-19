/*
 * =====================================================================================
 *
 *       Filename:  voltmeter.hpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  26/10/2025 18:51:53
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef VOLTMETER_HPP
#define VOLTMETER_HPP

#include <driver/gpio.h>
#include <Arduino.h>

typedef struct {
    float voltage; 
    float raw_voltage;
} VoltmeterInfo;


class Voltmeter {
    private:
        gpio_num_t pin;
        float upper_resistance;
        float lower_resistance;
        const float voltage_offset = 0.0;
        const float adc_resolution = 12; // in bits
        const float ref_voltage = 3.3;
    public:
        Voltmeter(gpio_num_t pin, float upper_resistance, float lower_resistance);
        VoltmeterInfo read(); 
};

#endif
