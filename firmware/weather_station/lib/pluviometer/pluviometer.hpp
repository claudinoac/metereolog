/*
 * =====================================================================================
 *
 *       Filename:  pluviometer.hpp
 *
 *    Description: Firmware Library for Rain Sensor (Pluviometer)
 *    Description: Firmware Library for Pluviometer (Rain Sensor) 
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

#define PLUVIOMETER_RESOLUTION 0.10
#define PLUVIOMETER_DIAMETER_MM 200.0

typedef struct {
    volatile unsigned long reed_count;
    float collected_rain_mm;
    float collected_volume_ml;
} PluviometerInfo;

class Pluviometer {
    public:
        Pluviometer(gpio_num_t pin);
        const float resolution_mm_per_pulse = PLUVIOMETER_RESOLUTION;
        const float diameter_mm = PLUVIOMETER_DIAMETER_MM;
        PluviometerInfo read();
        static Pluviometer *get_instance();
        void IRAM_ATTR interrupt_routine(); 
    private:
        gpio_num_t pin;
        unsigned long reed_count = 0;
        unsigned long last_reed;
        volatile unsigned long contact_bounce = 0;
};

#endif