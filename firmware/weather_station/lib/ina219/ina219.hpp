#ifndef INA219_H
#define INA219_H
#endif

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_INA219.h>

typedef struct {
   float voltage; //  as Volts
   float current; // as mA
   float power;   // as mW
} PowerInfo; 

class INA219 {
    public:
        INA219(TwoWire *i2c_channel);
        PowerInfo read();
    private:
        Adafruit_INA219 *sensor; 
};
