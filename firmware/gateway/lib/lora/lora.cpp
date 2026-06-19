#include "lora.hpp"

LoRa *lora_ptr = nullptr; 

void IRAM_ATTR on_receive() {
    if (LoRa::get_instance() != nullptr) {
        LoRa *instance = LoRa::get_instance();
        if (!instance->packet_received) {
            instance->packet_received = true;
        }
    }
};

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
            Serial.println("LoRa configured as a receptor...");
            this->radio->setPacketReceivedAction(on_receive); 
            this->radio->startReceive();
            // this->data_queue = xQueueCreate(10, 255*sizeof(char));
            // if (this->data_queue == NULL) {
            //     Serial.println("Panic while configuring queue. ");
            // }
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

void LoRa::handle_receive() {
};

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

float LoRa::get_rssi() {
    return this->radio->getRSSI();
}

float LoRa::get_snr() {
    return this->radio->getSNR();
}

String LoRa::get_message() {
    String packet;
    String full_message;
    String content;
    String device_id;   
    int message_idx;

    packet = this->get_packet();
    // Serial.print("First packet: "); Serial.println(packet);
    if (packet.length() == 0) {
        return "";
    }
    device_id = packet.substring(0, 31);
    message_idx = packet.substring(33).toInt();
    content = packet.substring(35, 255);

    if (message_idx != 0) {
        return "";
    }

    full_message += content;

    while(!full_message.endsWith("}}")) {
        packet = this->get_packet();
        // Serial.print("Subsequent packet: "); Serial.println(packet);
        if (packet.length() == 0) {
            continue;
        }
        String curr_device_id = packet.substring(0, 31);
        int curr_message_idx = packet.substring(33).toInt();
        if (curr_device_id == device_id && curr_message_idx > message_idx) {
            message_idx = curr_message_idx;
            content = packet.substring(35, 255);
            full_message += content;
        } else if (curr_device_id == device_id && curr_message_idx == message_idx) {
            continue;
        } else {
            return "";
        }
    }
    Serial.print("Full message: "); Serial.println(full_message);
    Serial.println();
    return full_message;
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

