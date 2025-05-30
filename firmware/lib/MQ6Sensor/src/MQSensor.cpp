#include "MQSensor.hpp"

MQ6Sensor::MQ6Sensor() {
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(this->adc_channel, ADC_ATTEN_DB_11);
    this->calibrate();
}

MQ6Info MQ6Sensor::read() {
    MQ6Info current_info = {0, 0.0f};

    current_info.raw_value = adc1_get_raw(this->adc_channel);

    if (adc_resolution > 0) {
        current_info.voltage = (current_info.raw_value / (float)adc_resolution) * reference_voltage;
    }

    return current_info;
}

float MQ6Sensor::calibrate() {
    const int samples = 10;
    const int delayBetweenSamples = 200; // ms
    float Rs_sum = 0.0f;
    int valid_samples = 0;

    for (int i = 0; i < samples; ++i) {
        MQ6Info info = this->read();

        if (info.voltage > 0.0f && info.voltage < reference_voltage) {
            float Rs = calculateRs(info.voltage);
            Rs_sum += Rs;
            valid_samples++;
        }
        delay(delayBetweenSamples);
    }

    if (valid_samples == 0) {
        return 0.0f; // Calibration failed
    }

    float Rs_avg = Rs_sum / valid_samples;
    // Ro é Rs no ar limpo / 10 para o MQ-6
    return Rs_avg / 10.0f;
}

float MQ6Sensor::read_ppm() {
    if (this->Ro <= 0.0f) {
        return 0.0f;
    }

    MQ6Info current_info = this->read();

    if (current_info.voltage <= 0.0f || current_info.voltage >= reference_voltage) {
        return 0.0f; // Invalid reading
    }

    float Rs = calculateRs(current_info.voltage);

    // Parâmetros da curva para GLP
    const float a = -0.42f;  // Inclinação
    const float b = 1.093f;  // Intercepto

    float ratio = Rs / this->Ro;
    float ppm_log = a * log10f(ratio) + b;

    return powf(10.0f, ppm_log);
}

float MQ6Sensor::calculateRs(float voltage) {
    if (voltage <= 0.0f) return FLT_MAX;
    return this->RL * ((reference_voltage - voltage) / voltage);
}
