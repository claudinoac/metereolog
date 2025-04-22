/*
 * =====================================================================================
 *
 *       Filename:  DHT.hpp
 *
 *    Description: Basic Wrapper for DHT sensor 
 *
 *        Version:  0.1.1
 *        Created:  05/04/2025 03:06:51
 *       Revision:  none
 *       Compiler:  clang
 *
 *         Author:  Alisson Claudino, 
 *   Organization:  UFSC
 *
 * =====================================================================================
 */
#ifndef DHTSENSOR_H
#define DHTSENSOR_H
#include <stdio.h>
#include <DHTNew.h>
#include <driver/gpio.h>

typedef struct {
    float temperature;
    float humidity;
} DHTInfo;

class DHTSensor {
    public:
        DHT *sensor;
        DHTSensor(gpio_num_t port, DHTModel_t model = DHT_MODEL_DHT11);
        DHTInfo read();
    private:
        unsigned long ts_read;
        unsigned long delta_time;
        unsigned long sampling_period;
};

#endif
