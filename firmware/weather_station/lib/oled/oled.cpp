#include "oled.hpp"

OLED::OLED(
    gpio_num_t sda_pin,
    gpio_num_t scl_pin,
    gpio_num_t rst_pin,
    gpio_num_t vext_pin,
    int width,
    int height
) {
    this->sda_pin = sda_pin;
    this->scl_pin = scl_pin;
    this->rst_pin = rst_pin;
    this->vext_pin = vext_pin;
    this->width = width;
    this->height = height;
    this->display = new U8G2_SSD1306_128X64_NONAME_F_SW_I2C(U8G2_R0, scl_pin, sda_pin, rst_pin);
};

void OLED::begin(){
    pinMode(this->vext_pin, OUTPUT);
    digitalWrite(this->vext_pin, LOW);
    delay(100);
    this->oled_i2c = new TwoWire(0);
    this->oled_i2c->begin(this->sda_pin, this->scl_pin);
    this->display->begin();
    this->display->clearBuffer();
    this->display->setFont(u8g2_font_6x10_tr);
    this->display->setDrawColor(1);
    this->display->sendBuffer();
};

void OLED::display_message(String message) {
    char *buffer = (char *)message.c_str();
    char *bufferPtr = strtok(buffer, "\n");
    int y_pos = 12;
    this->display->clearBuffer();
    this->display->setFont(u8g2_font_6x10_tr);
    while (bufferPtr != NULL) {
        this->display->setCursor(0, y_pos);
        this->display->print(bufferPtr);
        y_pos += 13;
        bufferPtr = strtok(NULL, "\n");
    }
    this->display->sendBuffer();
};
