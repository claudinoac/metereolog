#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <esp_log.h>
#include <sdkconfig.h>
#include <driver/gpio.h>
#include <Arduino.h>
#include "DHTSensor.hpp"

#define GPIO_PIN_4 (gpio_num_t) 4

DHTSensor *dht_sensor;
DHTInfo dht_reading;

void setup(){
    dht_sensor = new DHTSensor(GPIO_PIN_4, DHT_MODEL_DHT11);
    Serial.begin(115200);
};

void loop(){
    dht_reading = dht_sensor->read();
    Serial.print("\nTemperature: ");
    Serial.print(dht_reading.temperature);
    Serial.print(" °C");
    Serial.print("\nHumidity: ");
    Serial.print(dht_reading.humidity);
    Serial.print(" %");
};
