#include "lora.hpp"

LoRa *lora_ptr = nullptr; 

void IRAM_ATTR on_receive() {
    if (LoRa::get_instance() != nullptr) {
        LoRa *instance = LoRa::get_instance();
        if (!instance->packet_received) {
            instance->packet_received = true;
        }
    }
}

LoRa::LoRa(
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
) {
    this->nss_pin = nss_pin;
    this->dio1_pin = dio1_pin;
    this->rst_pin = rst_pin;
    this->busy_pin = busy_pin;
    this->band = band;
    this->bandwidth = bandwidth;
    this->spreading_factor = spreading_factor;
    this->coding_rate = coding_rate;
    this->max_power = max_power;
    this->mode = mode;
    lora_ptr = this;
};

bool LoRa::begin() {
    Serial.println(F("Configurando LoRa..."));
    this->radio_module = new Module(this->nss_pin, this->dio1_pin, this->rst_pin, this->busy_pin);
    this->radio = new SX1262(this->radio_module);
    int state = this->radio->begin(this->band);
    this->radio->setBandwidth(this->bandwidth);
    this->radio->setSpreadingFactor(this->spreading_factor);
    this->radio->setCodingRate(this->coding_rate);
    this->radio->setSyncWord(0x12);
    this->radio->setCRC(1);
    this->radio->fixedPacketLengthMode(255);
    this->radio->setOutputPower(this->max_power);

    if (state == RADIOLIB_ERR_NONE) {
        if (mode == Receptor) {
            this->radio->setPacketReceivedAction(on_receive); 
            this->radio->startReceive();
        }
    } else {
        Serial.println("Error while setting up LoRa radio->");
        return false;
    }
    return true;
}

LoRa* LoRa::get_instance() {
    return lora_ptr;
}

String LoRa::get_packet() {
    String received_data;
    while(!this->packet_received) {
        delay(5);
    };
    delay(100);
    int state = this->radio->readData(received_data); 
    this->packet_received = false;
    if (state != RADIOLIB_ERR_NONE) {
        Serial.print("Error while receiving data: " + String(state) + "\n\n");
        return "";
    }
    this->radio->startReceive(); 
    return received_data;
}

bool LoRa::send_message(String message) {
    Serial.print(F("Enviando pacote ")); Serial.println(message);
    int state = this->radio->transmit(message); 
    if (state == RADIOLIB_ERR_NONE) {
        return true;
    } else {
        Serial.print("Error while sending message. Error code: ");
        Serial.print(state);
        Serial.println();
        return false;
    }
}

