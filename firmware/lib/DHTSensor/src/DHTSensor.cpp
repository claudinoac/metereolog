/*
 * =====================================================================================
 *
 *       Filename:  DHTSensor.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/04/2025 04:06:41
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <Arduino.h>
#include "DHTSensor.hpp"


DHTSensor::DHTSensor(gpio_num_t port, DHTModel_t model) {
    this->sensor = new DHT((uint8_t)port, model);
    gpio_pad_select_gpio(port);
    gpio_set_direction(port, GPIO_MODE_INPUT);
    this->sensor->begin();
}

DHTInfo DHTSensor::read() {
    DHTInfo current_info = {0, 0};
    this->delta_time = millis() - this->ts_read;
    this->sampling_period = this->sensor->getMinimumSamplingPeriod();
    if (this->delta_time < this->sampling_period) {
        delay(this->sampling_period - this->delta_time);
    }
    current_info.temperature = this->sensor->readTemperature();
    current_info.humidity = this->sensor->readHumidity();
    this->ts_read = millis();
    return current_info;
}
