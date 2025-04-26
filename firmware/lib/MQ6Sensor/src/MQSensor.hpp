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
        MQ6Sensor(adc1_channel_t channel, float maxVoltage = 5.0f, int maxAdc = 4095);
        MQ6Info read();
        float calibrate(float RL = 4700.0f);
        float readPPM(float Ro, float RL = 4700.0f);

    private:
        adc1_channel_t adc_channel;
        float reference_voltage;
        int adc_resolution;

        float calculateRs(float voltage, float RL);
};

#endif
