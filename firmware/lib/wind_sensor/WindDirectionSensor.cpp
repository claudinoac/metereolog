#include "WindDirectionSensor.hpp"

WindDirectionSensor::WindDirectionSensor(gpio_num_t pin) : pin(pin), lastVoltage(0.0) {
    gpio_pad_select_gpio(pin);
    gpio_set_direction(pin, GPIO_MODE_INPUT);
}

WindDirectionInfo WindDirectionSensor::read() {
    WindDirectionInfo info = {0.0, 0, ""};
    int raw = analogRead((uint8_t)pin);
    info.voltage = raw * (3.3 / 4095.0);
    lastVoltage = info.voltage;

    // Determina ângulo
    if (info.voltage <= 2.90) info.angle = 0;
    else if (info.voltage <= 3.05) info.angle = 315;
    else if (info.voltage <= 3.25) info.angle = 270;
    else if (info.voltage <= 3.45) info.angle = 225;
    else if (info.voltage <= 3.75) info.angle = 180;
    else if (info.voltage <= 4.00) info.angle = 135;
    else if (info.voltage <= 4.25) info.angle = 90;
    else if (info.voltage <= 4.65) info.angle = 45;
    else info.angle = 0; // FAIL

    // Determina direção textual
    switch (info.angle) {
        case 0: info.direction = "Norte"; break;
        case 45: info.direction = "Nordeste"; break;
        case 90: info.direction = "Leste"; break;
        case 135: info.direction = "Sudeste"; break;
        case 180: info.direction = "Sul"; break;
        case 225: info.direction = "Sudoeste"; break;
        case 270: info.direction = "Oeste"; break;
        case 315: info.direction = "Noroeste"; break;
        default: info.direction = "FAIL";
    }

    return info;
}
