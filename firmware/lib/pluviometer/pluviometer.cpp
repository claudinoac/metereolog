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

Pluviometer *pluviometer_ptr = nullptr;

void IRAM_ATTR int_handler() {
    if (Pluviometer::get_instance() != nullptr) {
        Pluviometer::get_instance()->interrupt_routine();
    }
}

Pluviometer::Pluviometer(gpio_num_t pin) {
    this->pin = pin;
    pinMode(pin, INPUT_PULLUP);
    attachInterrupt(pin, int_handler, FALLING);
    pluviometer_ptr = this;
}

void Pluviometer::interrupt_routine() {
  if ((millis() - this->contact_bounce) > 0 ) { // debounce the switch contact.
    this->reed_count += 1;              // Adiciona 1 à cntagem de pulsos
    this->contact_bounce = millis();
  }
}

Pluviometer *Pluviometer::get_instance() {
    return pluviometer_ptr;
}

PluviometerInfo Pluviometer::read() {
    PluviometerInfo info = {0,0};
    info.collected_volume = (this->reed_count * 0.25) * 10;
    info.reed_count = this->reed_count;

    if(millis() - this->last_reed > 86400000) { 
        this->reed_count = 0;
        this->last_reed = millis(); 
    }

    return info;
}
