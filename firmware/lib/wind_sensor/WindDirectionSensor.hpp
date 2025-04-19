#ifndef WINDDIRECTIONSENSOR_H
#define WINDDIRECTIONSENSOR_H

#include <driver/gpio.h>
#include <Arduino.h>
#include <string>

typedef struct {
    float voltage;       // tensão lida
    int angle;          // ângulo da direção
    std::string direction; // direção em texto
} WindDirectionInfo;

class WindDirectionSensor {
    public:
        WindDirectionSensor(gpio_num_t pin);
        WindDirectionInfo read();
    private:
        gpio_num_t pin;
        float lastVoltage;
};

#endif
