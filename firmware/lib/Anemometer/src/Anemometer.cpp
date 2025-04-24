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
        gpio_install_isr_service(0); // instala apenas uma vez
        isr_service_installed = true;
    }

    gpio_isr_handler_add(inputPin, anemometer_isr_handler, (void*) inputPin);
}

WindInfo Anemometer::read() {
    WindInfo info;
    static unsigned long lastReadTime = millis();

    unsigned long now = millis();
    unsigned long elapsed = now - lastReadTime;

    noInterrupts();
    unsigned int count = global_pulse_count;
    global_pulse_count = 0;
    interrupts();

    float seconds = elapsed / 1000.0;
    float rotations = count / 2.0; // 2 pulsos = 1 volta
    float frequency = rotations / seconds;
    float v = 2 * PI * (radius / 100.0) * frequency;

    info.windSpeed = v;
    info.rotations = frequency;
    lastReadTime = now;

    return info;
}
