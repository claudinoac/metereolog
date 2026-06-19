#include "amperimeter.hpp" 

Amperimeter::Amperimeter(gpio_num_t pin, float upper_resistance, float lower_resistance, float conversion_rate, float base_voltage) {
    this->pin = pin;
    this->base_voltage = base_voltage;
    this->conversion_rate = conversion_rate;
    this->upper_resistance = upper_resistance;
    this->lower_resistance = lower_resistance;
    gpio_pad_select_gpio(pin);
    gpio_set_direction(pin, GPIO_MODE_INPUT);
    analogSetPinAttenuation(pin, ADC_11db);
};

AmperimeterInfo Amperimeter::read() {
    float voltage_factor = (this->lower_resistance + this->upper_resistance) / this->lower_resistance;
    int raw_value = analogRead(this->pin);
    float raw_voltage = this->voltage_offset + ((raw_value * this->ref_voltage) / this->adc_resolution);
    float measured_voltage = raw_voltage * voltage_factor;
    float current = (this->base_voltage - measured_voltage) / conversion_rate;
    return AmperimeterInfo{ current, raw_voltage, measured_voltage };
}
