#ifndef OLED_H
#define OLED_H
#endif

#include <driver/gpio.h>
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <U8g2lib.h>


class OLED {
    public:
        OLED(
            gpio_num_t sda_pin,
            gpio_num_t scl_pin,
            gpio_num_t rst_pin,
            gpio_num_t vext_pin,
            int width,
            int height
        );
        void begin();
        void display_message(String message);

    private:
        gpio_num_t sda_pin;
        gpio_num_t scl_pin;
        gpio_num_t rst_pin;
        gpio_num_t vext_pin;
        int width;
        int height;
        TwoWire *oled_i2c;

        U8G2_SSD1306_128X64_NONAME_F_SW_I2C *display;

};
