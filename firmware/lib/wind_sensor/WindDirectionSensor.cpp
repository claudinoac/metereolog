#include "WindDirectionSensor.hpp"
#include "driver/adc.h"

WindDirectionSensor::WindDirectionSensor(gpio_num_t pin) : pin(pin), lastVoltage(0.0) {
    analogSetPinAttenuation(pin, ADC_6db);
    gpio_pad_select_gpio(pin);
    gpio_set_direction(pin, GPIO_MODE_INPUT);
}

WindDirectionInfo WindDirectionSensor::read() {
    WindDirectionInfo info = {0.0, 0, ""};
    int raw = analogRead((uint8_t)pin);
    info.voltage = raw * (1.7 / 4095.0);
    lastVoltage = info.voltage;

    // Determines angle
    if (info.voltage >= 1.5 ) info.angle = 45; // NE
    else if (info.voltage >= 1.1) info.angle = 90; // E
    else if (info.voltage >= 0.8) info.angle = 135; // SE
    else if (info.voltage >= 0.7) info.angle = 180; // S
    else if (info.voltage >= 0.55) info.angle = 225; // SW
    else if (info.voltage >= 0.5) info.angle = 270; // W
    else if (info.voltage >= 0.4) info.angle = 315; // NW
    else info.angle = 0; // N

    // Determines direction
    switch (info.angle) {
        case 0: strcpy(info.direction, "N"); break;
        case 45: strcpy(info.direction, "NE"); break;
        case 90: strcpy(info.direction, "E"); break;
        case 135: strcpy(info.direction, "SE"); break;
        case 180: strcpy(info.direction, "S"); break;
        case 225: strcpy(info.direction, "SW"); break;
        case 270: strcpy(info.direction, "W"); break;
        case 315: strcpy(info.direction, "NW"); break;
        default: strcpy(info.direction, "Er");
    }

    return info;
}
