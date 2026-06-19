/*
 * =====================================================================================
 *
 *       Filename:  pluviometer.hpp
 *
 *    Description: Firmware Library for Rain Sensor (Pluviometer)
 *
 *        Version:  1.0
 *        Created:  18/10/2025 02:48:16
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alisson Claudino, 
 *   Organization:  UFSC
 *
 * =====================================================================================
 */
#ifndef PLUVIOMETER_HPP
#define PLUVIOMETER_HPP

#include <driver/gpio.h>
#include <Arduino.h>

typedef struct {
    unsigned long reed_count;
    float collected_volume;
} PluviometerInfo;

class Pluviometer {
    public:
        Pluviometer(gpio_num_t pin);
        PluviometerInfo read();
        static Pluviometer *get_instance();
        void IRAM_ATTR interrupt_routine(); 
    private:
        gpio_num_t pin;
        unsigned long reed_count = 0;
        unsigned long last_reed = 0;
        volatile unsigned long contact_bounce = 0;
};

#endif
