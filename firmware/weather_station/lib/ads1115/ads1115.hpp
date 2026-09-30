#ifndef ADS1115_H
#define ADS1115_H
#endif

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_ADS1X15.h>


class ADS1115 {
    public:
        ADS1115(TwoWire *i2c_channel, adsGain_t gain = GAIN_ONE);
        float read_channel(int channel);
    private:
        Adafruit_ADS1115 *sensor; 
};
