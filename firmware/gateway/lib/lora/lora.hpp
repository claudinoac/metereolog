#ifndef LORA_H
#define LORA_H
#endif

#include <RadioLib.h>
#include <driver/gpio.h>
#include <Arduino.h>

enum Mode {
    Transmitter,
    Receptor,
};


class LoRa {
    public:
        LoRa(
            gpio_num_t nss_pin,
            gpio_num_t dio1_pin,
            gpio_num_t rst_pin,
            gpio_num_t busy_pin,
            float band,
            float bandwidth,
            int spreading_factor,
            int coding_rate,
            float max_power,
            Mode mode
        );
        bool send_message(String message);
        static LoRa *get_instance();
        void handle_receive();
        bool begin();
        String get_message();
        float get_rssi();
        float get_snr();
        bool packet_received = false;
        String get_packet();

    private:
        SX1262 *radio = nullptr;
        Module *radio_module = nullptr;
        float current_rssi;
        float current_snr;
        QueueHandle_t data_queue;
        String current_message = "";

        gpio_num_t nss_pin;
        gpio_num_t dio1_pin;
        gpio_num_t rst_pin;
        gpio_num_t busy_pin;
        float band;
        float bandwidth;
        int spreading_factor;
        int coding_rate;
        float max_power;
        Mode mode;
        

};
