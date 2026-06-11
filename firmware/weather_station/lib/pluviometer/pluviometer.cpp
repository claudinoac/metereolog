/*
 * =====================================================================================
 *
 *       Filename:  pluviometer.cpp
 *
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
#include "pluviometer.hpp"

portMUX_TYPE pluviometer_mutex = portMUX_INITIALIZER_UNLOCKED;
Pluviometer *pluviometer_ptr = nullptr;

void IRAM_ATTR int_handler() {
    if (Pluviometer::get_instance() != nullptr) {
        Pluviometer::get_instance()->interrupt_routine();
    }
}

Pluviometer::Pluviometer(gpio_num_t pin) {
    this->pin = pin;
    this->last_reed = millis();
    pinMode(pin, INPUT_PULLUP);
    attachInterrupt(pin, int_handler, FALLING);
    pluviometer_ptr = this;
}

void Pluviometer::interrupt_routine() {
    portENTER_CRITICAL_ISR(&pluviometer_mutex);
    if ((millis() - this->contact_bounce) > 50 ) { // debounce the switch contact (50ms)
        this->reed_count += 1;              // add one pulse to the count
        this->contact_bounce = millis();
    }
    portEXIT_CRITICAL_ISR(&pluviometer_mutex);
}

Pluviometer *Pluviometer::get_instance() {
    return pluviometer_ptr;
}

PluviometerInfo Pluviometer::read() {
    PluviometerInfo info = {0,0,0};

    portENTER_CRITICAL(&pluviometer_mutex);
    info.reed_count = this->reed_count;
    if(millis() - this->last_reed > 86400000) { 
        this->reed_count = 0;
        this->last_reed = millis(); 
    }
    portEXIT_CRITICAL(&pluviometer_mutex);

    info.collected_rain_mm = info.reed_count * this->resolution_mm_per_pulse;
    info.collected_volume_ml = info.collected_rain_mm * (M_PI * this->diameter_mm * this->diameter_mm / 4.0) / 1000.0;

    return info;
}