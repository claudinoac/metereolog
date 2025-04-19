
#include "AnemometerSensor.hpp"

// Definição da variável estática (OBRIGATÓRIO)
AnemometerSensor* AnemometerSensor::instance = nullptr;

// Implementação do construtor
AnemometerSensor::AnemometerSensor(gpio_num_t pin) 
    : pin(pin), pulseCount(0), ts_read(0) {
    instance = this; // Atribui a instância atual
    
    gpio_pad_select_gpio(pin);
    gpio_set_direction(pin, GPIO_MODE_INPUT);
    gpio_set_pull_mode(pin, GPIO_PULLUP_ONLY);
    attachInterrupt(digitalPinToInterrupt(pin), isrHandler, FALLING);
}

// Implementação da ISR
void IRAM_ATTR AnemometerSensor::handleInterrupt() {
    pulseCount++;
}

// Implementação do método read
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

// Handler da ISR (definido fora da classe)
void IRAM_ATTR isrHandler() {
    if (AnemometerSensor::getInstance() != nullptr) {
        AnemometerSensor::getInstance()->handleInterrupt();
    }
}
