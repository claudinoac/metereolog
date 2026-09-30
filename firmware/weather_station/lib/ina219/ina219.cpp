/*
 * =====================================================================================
 *
 *       Filename:  ina219.cpp
 *
 *    Description: 
 *
 *
 *        Version:  1.0
 *        Created:  23/09/2026 14:47:32
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "ina219.hpp"

INA219::INA219(TwoWire *i2c_channel) {
    this->sensor = new Adafruit_INA219();
    unsigned status = this->sensor->begin(i2c_channel);
    int countdown = 10;
    while(!status) {
        Serial.println(F("Could not find a valid INA219 sensor, check wiring or try a different address!"));
        status = this->sensor->begin(i2c_channel);
        delay(2000);
        countdown -= 1;
        if (countdown < 1) {
            throw std::runtime_error("INA219 not configured.");
        }
    }
    this->sensor->setCalibration_16V_400mA();
    Serial.println("INA219 initialized successfully!");

}

PowerInfo INA219::read() {
    float voltage = this->sensor->getBusVoltage_V();
    float current = this->sensor->getCurrent_mA();
    float power = voltage * current;

    return PowerInfo { voltage, current, power };
}

