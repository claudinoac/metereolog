// RANSMISSOR OTIMIZADO - Heltec LoRa 32 V3
#include <Arduino.h>
#include <RadioLib.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <U8g2lib.h>
#include "lora.hpp"
#include "wifi.hpp"
#include "mqtt.hpp"
#include "oled.hpp"
#include "time.h"
#include "RTClib.h"
#include <ArduinoJson.h>

/* ============ WIFI CONFIGS ===================*/ 
const char* ssid = ENV_WIFI_SSID;
const char* password = ENV_WIFI_PASS;

MQTT *mqtt_client;
Wifi *wifi_client;

/* ============ BROKER CONFIGS ===================*/ 
const char *broker_user = ENV_BROKER_USER;
const char *broker_password = ENV_BROKER_PASS;
const char *broker_addr = ENV_BROKER_ADDR;
const int broker_port = atoi(ENV_BROKER_PORT);

const char *client_id = ENV_BROKER_CLIENT_ID;
const char *mqtt_topic = ENV_BROKER_TOPIC;

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

/* ================= OLED CONFIGS ===============*/

#define OLED_SDA (gpio_num_t) 17
#define OLED_SCL (gpio_num_t) 18
#define OLED_RST (gpio_num_t) 21
#define VEXT_PIN (gpio_num_t) 36
#define WIDTH  128
#define HEIGHT 64

OLED *display;

/* ================= RTC CONFIGS ===============*/

// #define RTC_SDA (gpio_num_t) 46
// #define RTC_SCL (gpio_num_t) 45
// 
// TwoWire *rtc_i2c;
// RTC_DS1307 *rtc;

/* ============ CLOCK CONFIGS ===================*/ 
const char* ntpServer = "pool.ntp.org";
unsigned long epochTime; 

/* ============ SENSOR CONFIGS ===================*/ 
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
const char *rssi_id = ENV_RSSI_ID;
const char *snr_id = ENV_SNR_ID;

JsonDocument doc;
char msg_buffer[1024]; 

/* ==============================================*/

long packet_id = 0;
// U8G2_SSD1306_128X64_NONAME_F_SW_I2C display(U8G2_R0, OLED_SCL, OLED_SDA, OLED_RESET);
LoRa *lora = new LoRa(NSS_LORA, DIO1_LORA, RESET_LORA, BUSY_LORA, BAND, BANDWIDTH, SPREADING_FACTOR, CODING_RATE, LORA_POWER, Receptor);



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
    float current;
    float uv_gauge;
} Packet;

void deserialize_packet(String raw_packet, Packet* p) {
    sscanf(
        raw_packet.c_str(),
        "%32s|%lu|%2[^|]|%f|%f|%f|%f|%f|%f|%f|%f|%f|%f",
        p->device_id,
        &p->timestamp,
        p->wind_dir,
        &p->dht_hum,
        &p->dht_temp,
        &p->wind_speed,
        &p->rain_gauge,
        &p->bmp_temp,
        &p->bmp_alt,
        &p->bmp_press,
        &p->voltage,
        &p->current,
        &p->uv_gauge
    );
}


void pretty_print_packet(const Packet* p, String snr, String rssi) {
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
  Serial.print("    snr = ");    Serial.print(snr);   Serial.println(",");
  Serial.print("    rssi = ");    Serial.print(rssi);   Serial.println(",");
  Serial.println("}");
}

void setup_display() {
    display = new OLED(OLED_SDA, OLED_SCL, OLED_RST, VEXT_PIN, WIDTH, HEIGHT);
    display->begin();
}

unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  while(!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
  }
  time(&now);
  return now;
}

// void setup_rtc() {
//     Serial.println("Starting configuration of RTC...");
//     rtc_i2c = new TwoWire(1);
//     rtc_i2c->begin(RTC_SDA, RTC_SCL);
//     rtc = new RTC_DS1307();
//     while (!rtc->begin(rtc_i2c)) {
//         Serial.println("Error while configuring RTC module");
//         delay(500);
//         break;
//     }
//     unsigned long timestamp = getTime();
//     if (!rtc->isrunning()) {
//         Serial.println("Configuring RTC...");
//         rtc->adjust(timestamp);
//     }
//     DateTime time = rtc->now();
//     // delay(5000);
// }

void setup() {
    Serial.begin(115200);
    setup_display();
    display->display_message("UFSC Metereolog Gateway v1!\nInitializing...");
    Serial.println("Initializing Receptor...");
    Serial.println("Display configured...");
    lora->begin();
    try {
        wifi_client = new Wifi((char *)ssid, (char *)password);
        mqtt_client = new MQTT(wifi_client, (char *)broker_addr, broker_port, (char *)broker_user, (char *)broker_password, (char *)client_id);
        configTime(0, 0, ntpServer);
        display->display_message("Connected to wifi " + String(ssid) + "and \n Broker " + String(broker_addr));
    } catch (const std::runtime_error& error) {
        Serial.println("Wifi and MQTT not configured.");        
        display->display_message("Wifi and MQTT not configured.");
    }
    delay(500);
}

String get_time_formatted() {
    time_t timestamp = getTime();
    struct tm *t = localtime(&timestamp);
    char buf[20];
    strftime(buf, sizeof(buf), "%d/%m/%y %H:%M", t);
    return String(buf);
}

void build_json(Packet *packet, String snr, String rssi) {
    JsonObject dht_temp = doc[dht_temp_id].to<JsonObject>();
    JsonObject dht_hum = doc[dht_hum_id].to<JsonObject>();
    dht_temp["value"] = packet->dht_temp;
    dht_temp["timestamp"] = packet->timestamp;
    dht_hum["value"] = packet->dht_hum;
    dht_hum["timestamp"] = packet->timestamp;

    JsonObject wind_dir = doc[anemoscope_id].to<JsonObject>();
    wind_dir["value"] = packet->wind_dir; 
    wind_dir["timestamp"] = packet->timestamp;

    JsonObject anemometer_data = doc[anemometer_id].to<JsonObject>();
    anemometer_data["value"] = packet->wind_speed;
    anemometer_data["timestamp"] = packet->timestamp;

    JsonObject pluviometer_data = doc[pluviometer_id].to<JsonObject>();
    pluviometer_data["value"] = packet->rain_gauge;
    pluviometer_data["timestamp"] = packet->timestamp;

    JsonObject bmp_temp_data = doc[bmp_temp_id].to<JsonObject>();
    JsonObject bmp_alt_data = doc[bmp_alt_id].to<JsonObject>();
    JsonObject bmp_press_data = doc[bmp_press_id].to<JsonObject>();
    bmp_temp_data["value"] = packet->bmp_temp;
    bmp_alt_data["value"] = packet->bmp_alt;
    bmp_press_data["value"] = packet->bmp_press;
    bmp_temp_data["timestamp"] = packet->timestamp;
    bmp_alt_data["timestamp"] = packet->timestamp;
    bmp_press_data["timestamp"] = packet->timestamp;

    JsonObject voltmeter_data = doc[voltmeter_id].to<JsonObject>();
    voltmeter_data["value"] = packet->voltage;
    voltmeter_data["timestamp"] = packet->timestamp;

    JsonObject amperimeter_data = doc[amperimeter_id].to<JsonObject>();
    amperimeter_data["value"] = packet->current;
    amperimeter_data["timestamp"] = packet->timestamp;

    JsonObject snr_data = doc[snr_id].to<JsonObject>();
    snr_data["value"] = snr;
    snr_data["timestamp"] = packet->timestamp;

    JsonObject rssi_data = doc[rssi_id].to<JsonObject>();
    rssi_data["value"] = rssi;
    rssi_data["timestamp"] = packet->timestamp;


    doc.shrinkToFit();
}

void loop() {
    // String message = lora->get_message();
    // if (message.length() > 0) {
    //    mqtt_client->publish((char *)mqtt_topic, message);
    // }
    // String packet = lora->get_packet();
    // Serial.println(packet);
    Packet packet;
    String raw_packet = lora->get_packet();
    if (raw_packet.length() > 0) {
        // Serial.println(raw_packet);
        String SNR = String(lora->get_snr());
        String RSSI = String(lora->get_rssi());
        deserialize_packet(raw_packet, &packet);
        pretty_print_packet(&packet, SNR, RSSI);
        // packet.timestamp = getTime();
        Serial.print("\n\n\n");
        build_json(&packet, SNR, RSSI);
        serializeJsonPretty(doc, Serial);
        if (mqtt_client != nullptr) {
            serializeJson(doc, msg_buffer);
            mqtt_client->publish((char *)mqtt_topic, msg_buffer);
        }
        display->display_message(
            "SNR:" + SNR 
            + " RSSI:" + RSSI 
            + "\nTime: " + String(packet.timestamp)
            + "\nTemp: " + String(packet.bmp_temp) + " " + String(packet.dht_temp)
            + "\nHum: " + String(packet.dht_hum)
            + " Alt:" + String(packet.bmp_alt)
            + "\nBar: " + String(packet.bmp_press)
        );
    }
}
