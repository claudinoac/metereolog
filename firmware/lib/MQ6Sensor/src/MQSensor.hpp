#ifndef MQ6SENSOR_H
#define MQ6SENSOR_H

#include <driver/adc.h>
#include <math.h>
#include <Arduino.h>
#include <cfloat>

typedef struct {
    int raw_value;
    float voltage;
} MQ6Info;

class MQ6Sensor {
    public:
        MQ6Sensor();
        MQ6Info read();
        float read_ppm();

    private:
        float Ro = 0.0f;
        float RL = 4700.0f;
        float reference_voltage = 3.9f;
        int adc_resolution = 4095;
        adc2_channel_t adc_channel = ADC2_CHANNEL_3;
        float calibrate();
        float calculateRs(float voltage);
};

#endif
