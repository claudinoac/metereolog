/*
 * =====================================================================================
 *
 *       Filename:  bmp.cpp
 *
 *    Description:  Firmware Library for BMP Sensor
 *      
 *
 *        Version:  1.0
 *        Created:  18/10/2025 03:44:31
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alisson Claudino, 
 *   Organization:  UFSC
 *
 * =====================================================================================
 */
#include "bmp.hpp"

BMPSensor::BMPSensor(TwoWire *i2c_channel, float sea_level_hpa) {
    this->bmp = new Adafruit_BMP280(i2c_channel);
    this->sea_level_hpa = sea_level_hpa;
    unsigned status;
    status = this->bmp->begin(0x76);
    int countdown = 10;
    while(!status) {
        Serial.println(F("Could not find a valid BMP280 sensor, check wiring or try a different address!"));
        Serial.print("SensorID was: 0x");
        Serial.println(bmp->sensorID(),16);
        Serial.print("Status was: "); Serial.println(status);
        status = this->bmp->begin(0x76);
        delay(2000);
        countdown -= 1;
        if (countdown < 1) {
            throw std::runtime_error("BMP not configured.");
        }
    }
    this->bmp->setSampling(
        Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
        Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
        Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
        Adafruit_BMP280::FILTER_X16,      /* Filtering. */
        Adafruit_BMP280::STANDBY_MS_500   /* Standby time. */
    );
}

BMPInfo BMPSensor::read() {
    float pressure = this->bmp->readPressure() / 100;
    float altitude = this->bmp->readAltitude(this->sea_level_hpa);
    float temperature = this->bmp->readTemperature();
    return BMPInfo { pressure, altitude, temperature };
}
