#include "voltmeter.hpp"
#include <Arduino.h>
#include <cmath>

Voltmeter::Voltmeter(gpio_num_t pin, float upper_resistance, float lower_resistance) {
    this->pin = pin;
    this->lower_resistance = lower_resistance;
    this->upper_resistance = upper_resistance;
    gpio_pad_select_gpio(pin);
    gpio_set_direction(pin, GPIO_MODE_INPUT);
    analogReadResolution(this->adc_resolution);
    analogSetPinAttenuation(pin, ADC_11db);
}

VoltmeterInfo Voltmeter::read() {
    int raw_value = analogRead(this->pin);
    float raw_voltage = this->voltage_offset + ((raw_value * this->ref_voltage) / pow(2,this->adc_resolution));
    Serial.print("RAW Voltage is: "); Serial.println(raw_voltage);
    float voltage = raw_voltage * ((this->lower_resistance + this->upper_resistance) / this->lower_resistance);
    Serial.print("Read Voltage is: "); Serial.println(voltage);

    return VoltmeterInfo { voltage, raw_voltage };
}
