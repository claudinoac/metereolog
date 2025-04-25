#include "Anemometer.hpp"

volatile unsigned int global_pulse_count = 0;

void IRAM_ATTR anemometer_isr_handler(void* arg) {
    global_pulse_count++;
}

Anemometer::Anemometer(gpio_num_t pin, float radius_cm) {
    this->inputPin = pin;
    this->radius = radius_cm;

    gpio_pad_select_gpio(inputPin);
    gpio_set_direction(inputPin, GPIO_MODE_INPUT);
    gpio_set_intr_type(inputPin, GPIO_INTR_POSEDGE);

    static bool isr_service_installed = false;
    if (!isr_service_installed) {
        gpio_install_isr_service(0); 
        isr_service_installed = true;
    }

    gpio_isr_handler_add(inputPin, anemometer_isr_handler, (void*) inputPin);
}

WindInfo Anemometer::read() {
    WindInfo info;
    static unsigned long lastReadTime = millis();

    unsigned long now = millis();
    unsigned long elapsed = now - lastReadTime;

    if (elapsed == 0) elapsed = 1;  // Evitar divisão por zero

    noInterrupts();
    unsigned int count = global_pulse_count;
    global_pulse_count = 0;
    interrupts();

    float seconds = elapsed / 1000.0;
    float frequency = count / seconds; // rotações por segundo
    float v_ms = 4 * PI * (radius/100) * frequency; // velocidade em m/s
    float v_kmh = v_ms * 3.6; // velocidade em km/h
    float rpm = frequency * 60;

    info.windSpeed = v_kmh;
    info.rotations = rpm;
    lastReadTime = now;

    return info;
}


