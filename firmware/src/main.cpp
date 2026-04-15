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
#include "time.h"

#include "main.html.hpp"
#include "dht_sensor.hpp"
#include "wifi.hpp"
#include "mqtt.hpp"
#include "anemoscope.hpp"
#include "anemometer.hpp"
#include "pluviometer.hpp"
#include "bmp.hpp"
#include "voltmeter.hpp"
#include "amperimeter.hpp"

#define ELEGANTOTA_USE_ASYNC_WEBSERVER 1

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

/* ============ OTA CONFIGS ===================*/ 
const char *ota_username = ENV_OTA_USER;
const char *ota_password = ENV_OTA_PASS;

/* ============ CLOCK CONFIGS ===================*/ 
const char* ntpServer = "pool.ntp.org";
unsigned long epochTime; 

/* ============ SENSOR CONFIGS ===================*/ 
#define ANEMOSCOPE_PIN (gpio_num_t) 35
#define ANEMOMETER_PIN (gpio_num_t) 15
#define DHT_11_PIN (gpio_num_t) 27
#define PLUVIOMETER_PIN (gpio_num_t) 33
#define SEA_LEVEL_HPA (1013.25)
#define VOLTMETER_PIN (gpio_num_t) 32
#define AMPERIMETER_PIN (gpio_num_t) 34
#define UVMETER_PIN (gpio_num_t) 25
#define VOLTMETER_UP_RES 30000
#define VOLTMETER_LOW_RES 7500
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
const char *uvmeter_id = ENV_UVMETER_ID;


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

/* ============ MISC VARIABLES ===================*/ 
JsonDocument doc;

AsyncWebServer server(80);

unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  while(!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
  }
  time(&now);
  return now;
}

void onOTAStart() {

}

void onOTAEnd(bool success) {

}

void onOTAProgress(size_t current, size_t final) {

}

void setup_sensors() {
    dht_sensor = new DHTSensor(DHT_11_PIN, DHT_MODEL_DHT11);
    anemoscope = new Anemoscope(ANEMOSCOPE_PIN);
    anemometer = new Anemometer(ANEMOMETER_PIN);
    pluviometer = new Pluviometer(PLUVIOMETER_PIN);
    bmp_sensor = new BMPSensor(SEA_LEVEL_HPA);
    voltmeter = new Voltmeter(VOLTMETER_PIN, VOLTMETER_UP_RES, VOLTMETER_LOW_RES);
    amperimeter = new Amperimeter(AMPERIMETER_PIN, AMPERIMETER_UP_RES, AMPERIMETER_LOW_RES);
}

void setup_network() {
    wifi_client = new Wifi((char *)ssid, (char *)password);
    mqtt_client = new MQTT(wifi_client, (char *)broker_addr, broker_port, (char *)broker_user, (char *)broker_password, (char *)client_id);
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
    Serial.print("Climate monitoring system started\n");
    setup_sensors();
    setup_network();
    setup_webserver();

    configTime(0, 0, ntpServer);
    delay(1000);
};

void read_sensors() {
    JsonObject dht_temp = doc[dht_temp_id].to<JsonObject>();
    JsonObject dht_hum = doc[dht_hum_id].to<JsonObject>();
    dht_reading = dht_sensor->read();
    dht_temp["value"] = dht_reading.temperature;
    dht_temp["timestamp"] = getTime();
    dht_hum["value"] = dht_reading.humidity;
    dht_hum["timestamp"] = dht_temp["timestamp"];

    JsonObject wind_dir = doc[anemoscope_id].to<JsonObject>();
    wind_direction_reading = anemoscope->read();
    wind_dir["value"] = wind_direction_reading.direction; 
    wind_dir["timestamp"] = getTime();

    JsonObject anemometer_data = doc[anemometer_id].to<JsonObject>();
    anemometer_reading = anemometer->read();
    anemometer_data["value"] = anemometer_reading.wind_speed;
    anemometer_data["timestamp"] = getTime();

    JsonObject pluviometer_data = doc[pluviometer_id].to<JsonObject>();
    pluviometer_reading = pluviometer->read();
    pluviometer_data["value"] = pluviometer_reading.collected_volume;
    anemometer_data["timestamp"] = getTime();

    JsonObject bmp_temp_data = doc[bmp_temp_id].to<JsonObject>();
    JsonObject bmp_alt_data = doc[bmp_alt_id].to<JsonObject>();
    JsonObject bmp_press_data = doc[bmp_press_id].to<JsonObject>();
    bmp_reading = bmp_sensor->read();
    bmp_temp_data["value"] = bmp_reading.temperature;
    bmp_alt_data["value"] = bmp_reading.altitude;
    bmp_press_data["value"] = bmp_reading.pressure;
    bmp_temp_data["timestamp"] = getTime();
    bmp_alt_data["timestamp"] = bmp_temp_data["timestamp"];
    bmp_press_data["timestamp"] = bmp_temp_data["timestamp"];

    JsonObject voltmeter_data = doc[voltmeter_id].to<JsonObject>();
    voltmeter_reading = voltmeter->read();
    voltmeter_data["value"] = voltmeter_reading.voltage;
    voltmeter_data["raw_value"] = voltmeter_reading.raw_voltage;
    voltmeter_data["timestamp"] = getTime();

    JsonObject amperimeter_data = doc[amperimeter_id].to<JsonObject>();
    amperimeter_reading = amperimeter->read();
    amperimeter_data["value"] = amperimeter_reading.current;
    amperimeter_data["raw_voltage"] = amperimeter_reading.raw_voltage;
    amperimeter_data["measured_voltage"] = amperimeter_reading.measured_voltage;
    amperimeter_data["timestamp"] = getTime();

    doc.shrinkToFit();
}

void loop() {
    read_sensors();
    loop_webserver();
    serializeJsonPretty(doc, Serial);
    mqtt_client->publish((char *)mqtt_topic, doc);
}
