#ifndef BMP_HPP
#define BMP_HPP
#endif

#include <driver/gpio.h>
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>

typedef struct {
   float pressure;
   float altitude;
   float temperature;
} BMPInfo;

class BMPSensor {
    public:
        BMPSensor(TwoWire *i2c_channel, float sea_level_hpa);
        BMPInfo read();
    private:
        Adafruit_BMP280 *bmp;
        unsigned status = 0;
        float sea_level_hpa;
};
