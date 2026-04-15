#include "anemometer.hpp"
#include <Arduino.h>

Anemometer* anemometer_ptr = nullptr;

// Interrupt routine Handler (defined outside class)
void IRAM_ATTR isrHandler() {
    if (Anemometer::getInstance() != nullptr) {
        Anemometer::getInstance()->handleInterrupt();
    }
}

Anemometer::Anemometer(gpio_num_t pin) 
    : pin(pin), pulseCount(0), ts_read(0) {
    anemometer_ptr = this; // Turn this class into a singleton
    
    pinMode(pin, INPUT_PULLUP);
    gpio_pad_select_gpio(pin);
    gpio_set_direction(pin, GPIO_MODE_INPUT);
    gpio_set_pull_mode(pin, GPIO_PULLUP_ONLY);
    attachInterrupt(digitalPinToInterrupt(pin), isrHandler, FALLING);
}

Anemometer *Anemometer::getInstance() {
    return anemometer_ptr;
}

// interrupt handler to count pulses
void IRAM_ATTR Anemometer::handleInterrupt() {
    pulseCount++;
}

AnemometerInfo Anemometer::read() {
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

