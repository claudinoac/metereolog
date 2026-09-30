#include "ads1115.hpp"

ADS1115::ADS1115(TwoWire *i2c_channel, adsGain_t gain) {
    this->sensor = new Adafruit_ADS1115();
    this->sensor->begin(0x48, i2c_channel);
    this->sensor->setGain(gain);
    i2c_channel->beginTransmission(0x48);
    if(i2c_channel->endTransmission() != 0) {
        Serial.println(F("Could not find a valid ADS1115 sensor, check wiring or try a different address!"));
        throw std::runtime_error("ADS1115 not configured.");
    }
    this->sensor->setDataRate(RATE_ADS1115_860SPS);
    Serial.println("ADS1115 sensor configured.");
};


float ADS1115::read_channel(int channel) {
    Serial.print("Reading adc channel ");
    int16_t adc_raw_result = this->sensor->readADC_SingleEnded(channel);
    Serial.println(adc_raw_result);
    return this->sensor->computeVolts(adc_raw_result);
};
