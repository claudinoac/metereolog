#include "AnemometerSensor.hpp"
#include <Arduino.h>

AnemometerSensor* anemometer = nullptr;

// Interrupt routine Handler (defined outside class)
void IRAM_ATTR isrHandler() {
    if (AnemometerSensor::getInstance() != nullptr) {
        AnemometerSensor::getInstance()->handleInterrupt();
    }
}

AnemometerSensor::AnemometerSensor(gpio_num_t pin) 
    : pin(pin), pulseCount(0), ts_read(0) {
    anemometer = this; // Turn this class into a singleton
    
    gpio_pad_select_gpio(pin);
    gpio_set_direction(pin, GPIO_MODE_INPUT);
    gpio_set_pull_mode(pin, GPIO_PULLUP_ONLY);
    attachInterrupt(digitalPinToInterrupt(pin), isrHandler, FALLING);
}

AnemometerSensor *AnemometerSensor::getInstance() {
    return anemometer;
}

// interrupt handler to count pulses
void IRAM_ATTR AnemometerSensor::handleInterrupt() {
    pulseCount++;
}

AnemometerInfo AnemometerSensor::read() {
    AnemometerInfo info = {0, 0};
    delay(1000); // Medição por 1 segundo
    
    noInterrupts();
    unsigned int pulses = pulseCount;
    pulseCount = 0;
    interrupts();
    
    info.wind_speed = pulses * 2.4; // 1 Hz = 2.4 km/h
    info.pulses = pulses;
    ts_read = millis();
    
    return info;
}

