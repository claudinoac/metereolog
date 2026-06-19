#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <esp_log.h>
#include <sdkconfig.h>
#include <driver/gpio.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>
#include <spi.h>
#include <SD.h>
#include <esp_system.h>

#include "main.html.hpp"
#include "dht_sensor.hpp"
#include "wifi.hpp"
#include "anemoscope.hpp"
#include "anemometer.hpp"
#include "pluviometer.hpp"
#include "bmp.hpp"
#include "voltmeter.hpp"
#include "amperimeter.hpp"
#include "lora.hpp"
#include "oled.hpp"
#include "rtc.hpp"

#define ELEGANTOTA_USE_ASYNC_WEBSERVER 1

/* ============ WIFI CONFIGS ===================*/ 
const char* ap_ssid = ENV_WIFI_AP_SSID;
const char* ap_pwd = ENV_WIFI_AP_PWD;
const char* ssid = ENV_WIFI_SSID;
const char* password = ENV_WIFI_PASS;

Wifi *wifi_client;

/* ============ OTA CONFIGS ===================*/ 
const char *ota_username = ENV_OTA_USER;
const char *ota_password = ENV_OTA_PASS;

/* ============ CLOCK CONFIGS ===================*/ 
unsigned long epochTime; 

/* ================= LORA CONFIGS ===============*/
#define NSS_LORA   (gpio_num_t)   8
#define DIO1_LORA  (gpio_num_t)   14
#define RESET_LORA (gpio_num_t)   12
#define BUSY_LORA  (gpio_num_t)   13
#define BAND          915.6
#define BANDWIDTH     62.5
#define SPREADING_FACTOR 10
#define CODING_RATE   4/5
#define LORA_POWER    22.0
const char *device_id = ENV_DEVICE_ID;


LoRa *lora;

/* ================= OLED CONFIGS ===============*/

#define OLED_SDA  (gpio_num_t)  17
#define OLED_SCL  (gpio_num_t)  18
#define OLED_RST  (gpio_num_t)  21
#define VEXT_PIN  (gpio_num_t)  36
#define WIDTH  128
#define HEIGHT 64

OLED *display;

/*  ==============  I2C Config ================o */

#define I2C_SDA (gpio_num_t) 7
#define I2C_SCL (gpio_num_t) 6

TwoWire *i2c_channel = new TwoWire(1);

/* ============ SDCARD CONFIGS ===================*/ 

#define SD_SCLK 48 // SCK
#define SD_MISO 33 // MISO
#define SD_MOSI 47 // MOSI
#define SD_CS 26 // CS

SPIClass *sd_spi = nullptr; 

/* ============ SENSOR CONFIGS ===================*/ 
#define ANEMOSCOPE_PIN (gpio_num_t) 2
#define ANEMOMETER_PIN (gpio_num_t) 5
#define DHT_PIN (gpio_num_t) 34
#define PLUVIOMETER_PIN (gpio_num_t) 0
#define SEA_LEVEL_HPA (1013.25)
#define VOLTMETER_PIN (gpio_num_t) 3
#define AMPERIMETER_PIN (gpio_num_t) 4
#define VOLTMETER_UP_RES 200000
#define VOLTMETER_LOW_RES 51000
#define AMPERIMETER_UP_RES 1000
#define AMPERIMETER_LOW_RES 2000

const char *dht_temp_id = ENV_DHT_TEMP_ID;
const char *dht_hum_id = ENV_DHT_HUM_ID;
const char *anemometer_id = ENV_ANEMOMETER_ID;
const char *anemoscope_id = ENV_ANEMOSCOPE_ID;
const char *pluviometer_id = ENV_PLUVIOMETER_ID;
const char *bmp_temp_id = ENV_BMP_TEMP_ID;
const char *bmp_alt_id = ENV_BMP_ALT_ID;
const char *bmp_press_id = ENV_BMP_PRESS_ID;
const char *voltmeter_id = ENV_VOLTMETER_ID;
const char *amperimeter_id = ENV_AMPERIMETER_ID;


DHTSensor *dht_sensor;
DHTInfo dht_reading;

Anemometer *anemometer;
AnemometerInfo anemometer_reading;

Anemoscope *anemoscope;
WindDirectionInfo wind_direction_reading;

Pluviometer *pluviometer;
PluviometerInfo pluviometer_reading;

BMPSensor *bmp_sensor;
BMPInfo bmp_reading;

Voltmeter *voltmeter;
VoltmeterInfo voltmeter_reading;

Amperimeter *amperimeter;
AmperimeterInfo amperimeter_reading;

RTC *rtc;

/* ============ MISC VARIABLES ===================*/ 

typedef struct __attribute__((packed)) {
    char device_id[33];
    unsigned long timestamp;
    char wind_dir[3];
    float dht_hum;
    float dht_temp;
    float wind_speed;
    float rain_gauge;
    float bmp_temp;
    float bmp_alt;
    float bmp_press;
    float voltage;
    float raw_voltage;
    float current;
} Packet;

JsonDocument doc;

AsyncWebServer server(80);

char msg_buffer[255]; 


String serialize_packet(const Packet* p) {
    String raw_packet = "";
    raw_packet += String(p->device_id) + "|";
    raw_packet += String(p->timestamp) + "|";
    raw_packet += String(p->wind_dir) + "|";
    raw_packet += String(p->dht_hum) + "|";
    raw_packet += String(p->dht_temp) + "|";
    raw_packet += String(p->wind_speed) + "|";
    raw_packet += String(p->rain_gauge) + "|";
    raw_packet += String(p->bmp_temp) + "|";
    raw_packet += String(p->bmp_alt) + "|";
    raw_packet += String(p->bmp_press) + "|";
    raw_packet += String(p->voltage) + "|";
    raw_packet += String(p->current);
    return raw_packet;
}

void pretty_print_packet(const Packet* p) {
  Serial.println("Packet {");
  Serial.print("    device_id = ");   Serial.print(p->device_id);  Serial.println(",");
  Serial.print("    timestamp = ");   Serial.print(p->timestamp);  Serial.println(",");
  Serial.print("    wind_dir = ");    Serial.print(p->wind_dir);   Serial.println(",");
  Serial.print("    dht_hum = ");     Serial.print(p->dht_hum);    Serial.println(",");
  Serial.print("    dht_temp = ");    Serial.print(p->dht_temp);   Serial.println(",");
  Serial.print("    wind_speed = ");  Serial.print(p->wind_speed); Serial.println(",");
  Serial.print("    rain_gauge = ");  Serial.print(p->rain_gauge); Serial.println(",");
  Serial.print("    bmp_temp = ");    Serial.print(p->bmp_temp);   Serial.println(",");
  Serial.print("    bmp_alt = ");     Serial.print(p->bmp_alt);    Serial.println(",");
  Serial.print("    bmp_press = ");   Serial.print(p->bmp_press);  Serial.println(",");
  Serial.print("    voltage = ");     Serial.print(p->voltage);    Serial.println(",");
  Serial.print("    current = ");     Serial.print(p->current);    Serial.println(",");
  Serial.println("}");
}



void onOTAStart() {
    display->display_message("Updating firmware...");
}

void onOTAEnd(bool success) {
    display->display_message("Update finished. \nRebooting...");
}

void onOTAProgress(size_t current, size_t final) {
    // display->display_message("Updating firmware...\nProgress: " + String(float(current) / float(final)));
}

void setup_i2c() {
    // pinMode(I2C_SCL, OUTPUT);
    // for (int i = 0; i < 8; i++) {
    //     digitalWrite(I2C_SCL, HIGH);
    //     delayMicroseconds(3);
    //     digitalWrite(I2C_SCL, LOW);
    //     delayMicroseconds(3);
    // }
    // pinMode(I2C_SCL, INPUT_PULLUP);
    // pinMode(I2C_SDA, INPUT_PULLUP);
    if (!i2c_channel->begin(I2C_SDA, I2C_SCL, 100000)) {
        Serial.println("Cannot initialize custom bmp I2C Channel");
    };
}

void setup_sensors() {
    dht_sensor = new DHTSensor(DHT_PIN, DHT_MODEL_DHT22);
    anemoscope = new Anemoscope(ANEMOSCOPE_PIN);
    anemometer = new Anemometer(ANEMOMETER_PIN);
    pluviometer = new Pluviometer(PLUVIOMETER_PIN);
    bmp_sensor = new BMPSensor(i2c_channel, SEA_LEVEL_HPA);
    voltmeter = new Voltmeter(VOLTMETER_PIN, VOLTMETER_UP_RES, VOLTMETER_LOW_RES);
    amperimeter = new Amperimeter(AMPERIMETER_PIN, AMPERIMETER_UP_RES, AMPERIMETER_LOW_RES);

}

void setup_rtc() {
    rtc = new RTC(i2c_channel);
}

// void diagnoseSdFailure() {
//     Serial.println("\n--- SD Diagnostics ---");
// 
//     // 1. Check if SPI bus responds at all (pull CS low, send dummy byte, check MISO)
//     sd_spi->begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);
//     pinMode(SD_CS, OUTPUT);
//     digitalWrite(SD_CS, HIGH);
//     delay(100);
// 
//     // Send 10 dummy bytes with CS high (required SD init sequence)
//     for (int i = 0; i < 10; i++) {
//         sd_spi->transfer(0xFF);
//     }
// 
//     digitalWrite(SD_CS, LOW);
//     delay(10);
//     uint8_t response = sd_spi->transfer(0xFF);
//     digitalWrite(SD_CS, HIGH);
// 
//     Serial.printf("SPI dummy byte response: 0x%02X (0xFF = line idle/ok, 0x00 = MISO stuck low)\n", response);
// 
//     // 2. Try SD init at very low speed (400 kHz is SD spec for init)
//     Serial.println("\nTrying SD.begin() at 400kHz...");
//     if (SD.begin(SD_CS, *sd_spi, 400000)) {
//         Serial.println("SUCCESS at 400kHz — your clock speed was too high");
//         return;
//     }
// 
//     // 3. Try with FSPI instead
//     Serial.println("Failed. Trying FSPI bus...");
//     SPIClass spi2 = SPIClass(FSPI);
//     spi2.begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);
//     if (SD.begin(SD_CS, spi2, 400000)) {
//         Serial.println("SUCCESS with FSPI — use SPIClass(FSPI) in your code");
//         return;
//     }
// 
//     // 4. Check card type after a failed begin
//     Serial.println("Failed both buses. Checking card type...");
//     uint8_t cardType = SD.cardType();
//     if (cardType == CARD_NONE) {
//         Serial.println("Card type: NONE — card not detected at all");
//         Serial.println("  -> Check wiring, CS pin, and 3.3V power");
//     } else if (cardType == CARD_UNKNOWN) {
//         Serial.println("Card type: UNKNOWN — card detected but not recognized");
//         Serial.println("  -> Try reformatting card as FAT32");
//     } else {
//         Serial.printf("Card type: %d — card found but mount still failed\n", cardType);
//         Serial.println("  -> Filesystem may be corrupted, try reformatting");
//     }
// 
//     // 5. MISO pin state check
//     pinMode(SD_MISO, INPUT);
//     int misoState = digitalRead(SD_MISO);
//     Serial.printf("\nMISO pin state (idle): %s\n", misoState ? "HIGH (ok)" : "LOW (stuck — bad wiring or short)");
// 
//     // 6. CS pin state check  
//     pinMode(SD_CS, INPUT_PULLUP);
//     delay(10);
//     int csState = digitalRead(SD_CS);
//     Serial.printf("CS pin state: %s\n", csState ? "HIGH (ok)" : "LOW (stuck — check for short)");
// }

void setup_sdcard() {
    sd_spi = new SPIClass(HSPI);
    sd_spi->begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);
    pinMode(SD_CS, OUTPUT);
    digitalWrite(SD_CS, HIGH);
    Serial.printf("SPI initialized on SCLK=%d MISO=%d MOSI=%d CS=%d\n", SD_SCLK, SD_MISO, SD_MOSI, SD_CS);
    int countdown = 3;
    while (!SD.begin(SD_CS, *sd_spi, 4000000)) {
        Serial.println("Card Mount Failed. Retrying...");
        delay(500);
        countdown -= 1;
        if (countdown < 1) {
            // diagnoseSdFailure();
            throw std::runtime_error("SDCARD not configured.");
        }
    }
    Serial.println("Card Mounted");
    File root = SD.open("/");
    if (root) {
        root.close();
        Serial.println("SD ready.");
    }
}

bool write_line(String filename, String line) {
    File file = SD.open("/" + filename, FILE_APPEND);
    if (file) {
        Serial.println("Writing to file: " + filename);
        file.println(line);
        file.close();
        Serial.println("Written to file!");
        return true;
    }
    Serial.println("Cannot open file: " + filename);
    return false;
}

void setup_network() {
    lora = new LoRa(NSS_LORA, DIO1_LORA, RESET_LORA, BUSY_LORA, BAND, BANDWIDTH, SPREADING_FACTOR, CODING_RATE, LORA_POWER, Receptor);
    wifi_client = new Wifi((char *)ssid, (char *)password, (char *)ap_ssid, (char *)ap_pwd);
    lora->begin();
}

void setup_display() {
    display = new OLED(OLED_SDA, OLED_SCL, OLED_RST, VEXT_PIN, WIDTH, HEIGHT);
    display->begin();
}

void setup_webserver() {
  server.on("/stats", HTTP_GET, [](AsyncWebServerRequest *request) {
    JsonDocument stats_doc;
    stats_doc["timestamp"] = doc[dht_temp_id]["timestamp"];

    stats_doc["anemometer"] = doc[anemometer_id]["value"] ? doc[anemometer_id]["value"] : 0;
    stats_doc["wind_dir"] = doc[anemoscope_id]["value"];
    stats_doc["voltmeter"] = doc[voltmeter_id]["value"];
    stats_doc["amperimeter"] = doc[amperimeter_id]["value"];

    JsonObject bmp = stats_doc["bmp"].to<JsonObject>();
    bmp["temperature"] = doc[bmp_temp_id]["value"];
    bmp["pressure"] = doc[bmp_press_id]["value"];
    bmp["altitude"] = doc[bmp_alt_id]["value"];

    JsonObject dht_11 = stats_doc["dht_11"].to<JsonObject>();
    dht_11["temperature"] = doc[dht_temp_id]["value"];
    dht_11["humidity"] = doc[dht_hum_id]["value"];
    
    String stats_output;
    serializeJson(stats_doc, stats_output);
    request->send(200, "application/json", stats_output);
  });

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", index_html);
  });
 
  ElegantOTA.begin(&server);
  ElegantOTA.setAuth(ota_username, ota_password);
  ElegantOTA.onStart(onOTAStart);
  ElegantOTA.onProgress(onOTAProgress);
  ElegantOTA.onEnd(onOTAEnd);
  server.begin();
  Serial.println("HTTP server started");
}

void loop_webserver() {
  ElegantOTA.loop();
}

void setup(){
    Serial.begin(115200);
    Serial.println("Metereolog Weather Station ARU 01 Starting...");
    setup_display();
    display->display_message("Metereolog Weather Station\nARU 01 Starting...");
    try {
        setup_i2c();
        setup_network();
        setup_rtc();
        setup_sensors();
    } catch (const std::runtime_error& error) {
        esp_restart();
    }
    setup_sdcard();
    // setup_webserver();
    display->display_message("Configured!");
};

Packet read_sensors() {
    Packet packet;
    time_t current_time = rtc->get_time();
    packet.timestamp = current_time;
    // packet.timestamp = 0;
    strcpy(packet.device_id, device_id);
    dht_reading = dht_sensor->read();
    packet.dht_temp = dht_reading.temperature;
    packet.dht_hum = dht_reading.humidity; 
    wind_direction_reading = anemoscope->read();
    strcpy(packet.wind_dir, wind_direction_reading.direction);
    anemometer_reading = anemometer->read();
    packet.wind_speed = anemometer_reading.wind_speed;
    pluviometer_reading = pluviometer->read();
    packet.rain_gauge = pluviometer_reading.collected_volume;
    bmp_reading = bmp_sensor->read();
    packet.bmp_temp = bmp_reading.temperature;
    packet.bmp_alt = bmp_reading.altitude;
    packet.bmp_press = bmp_reading.pressure;
    voltmeter_reading = voltmeter->read();
    packet.voltage = voltmeter_reading.voltage;
    packet.raw_voltage = voltmeter_reading.raw_voltage;
    amperimeter_reading = amperimeter->read();
    packet.current = amperimeter_reading.current;
    return packet;
    // JsonObject dht_temp = doc[dht_temp_id].to<JsonObject>();
    // JsonObject dht_hum = doc[dht_hum_id].to<JsonObject>();
    // dht_temp["value"] = dht_reading.temperature;
    // dht_temp["timestamp"] = packet.timestamp;
    // dht_hum["value"] = dht_reading.humidity;
    // dht_hum["timestamp"] = dht_temp["timestamp"];

    // JsonObject wind_dir = doc[anemoscope_id].to<JsonObject>();
    // wind_dir["value"] = wind_direction_reading.direction; 
    // wind_dir["timestamp"] = packet.timestamp;

    // JsonObject anemometer_data = doc[anemometer_id].to<JsonObject>();
    // anemometer_data["value"] = anemometer_reading.wind_speed;
    // anemometer_data["timestamp"] = packet.timestamp;

    // JsonObject pluviometer_data = doc[pluviometer_id].to<JsonObject>();
    // pluviometer_data["value"] = pluviometer_reading.collected_volume;
    // anemometer_data["timestamp"] = packet.timestamp;

    // JsonObject bmp_temp_data = doc[bmp_temp_id].to<JsonObject>();
    // JsonObject bmp_alt_data = doc[bmp_alt_id].to<JsonObject>();
    // JsonObject bmp_press_data = doc[bmp_press_id].to<JsonObject>();
    // bmp_temp_data["value"] = bmp_reading.temperature;
    // bmp_alt_data["value"] = bmp_reading.altitude;
    // bmp_press_data["value"] = bmp_reading.pressure;
    // bmp_temp_data["timestamp"] = packet.timestamp;
    // bmp_alt_data["timestamp"] = bmp_temp_data["timestamp"];
    // bmp_press_data["timestamp"] = bmp_temp_data["timestamp"];

    // JsonObject voltmeter_data = doc[voltmeter_id].to<JsonObject>();
    // voltmeter_data["value"] = voltmeter_reading.voltage;
    // voltmeter_data["raw_value"] = voltmeter_reading.raw_voltage;
    // voltmeter_data["timestamp"] = packet.timestamp;

    // JsonObject amperimeter_data = doc[amperimeter_id].to<JsonObject>();
    // amperimeter_data["value"] = amperimeter_reading.current;
    // amperimeter_data["raw_voltage"] = amperimeter_reading.raw_voltage;
    // amperimeter_data["measured_voltage"] = amperimeter_reading.measured_voltage;
    // amperimeter_data["timestamp"] = packet.timestamp;

    // doc.shrinkToFit();

}

void loop() {
    Packet packet = read_sensors();
    // loop_webserver();
    // serializeJsonPretty(doc, Serial);
    // serializeJson(doc, msg_buffer);
    String raw_packet = serialize_packet(&packet);
    // Serial.print("RAW Message: "); Serial.println(raw_packet);
    write_line("aru_0_data.txt", raw_packet);
    lora->send_message(raw_packet);
    // String message = lora->get_packet();
    display->display_message(
        "Time: " + rtc->get_time_formatted()
        + "\nTemp: " + String(packet.bmp_temp) + " " + String(packet.dht_temp)
        + "\nHum: " + String(packet.dht_hum)
        + " Press: " + String(packet.bmp_press)
        + "\nVolt" + String(packet.voltage) + " " + String(packet.raw_voltage)
        + "\nAlt: " + String(packet.bmp_alt)
        + " Dir: " + packet.wind_dir
        + "\n Wind Speed: : " + packet.wind_speed
        + "\n Volt: " + String(packet.voltage) + "V Amp: " + String(packet.current) + "A"
    );
    // String time = rtc->get_time_formatted();
    // display->display_message("Time: " + time);
    // Serial.print("Time: ");
    // Serial.println(time);
    delay(100);
}
