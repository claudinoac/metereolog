#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <esp_log.h>
#include <sdkconfig.h>
#include <driver/gpio.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include "time.h"
#include "DHTSensor.hpp"
#include "wifi.hpp"
#include "mqtt.hpp"
#include "WindDirectionSensor.hpp"
#include "Anemometer.hpp"
#include "MQSensor.hpp"


#define WIND_DIRECTION_PIN (gpio_num_t) 32
#define ANEMOMETER_PIN (gpio_num_t) 2
#define DHT_11_PIN (gpio_num_t) 4



const char* ssid = "Alisson";
const char* password = "err0r404";
const char *broker_addr = "0.tcp.ngrok.io";
const int broker_port = 13366;
const char *broker_user = "metereolog";
const char *broker_password = "metereolog";
const char *client_id = "esp32_met_cit";
// char *mqtt_topic = "mqtt.e7320819fa054169afe084caf3f5eb78.6d2673ca040845568605c9a738a8bae3";
const char *mqtt_topic = "mqtt.e7320819fa054169afe084caf3f5eb78.ff0ee09617cf44a8a14c237fb1404a34";
const char* ntpServer = "pool.ntp.org";
unsigned long epochTime; 


MQTT *mqtt_client;
Wifi *wifi_client;

DHTSensor *dht_sensor;
DHTInfo dht_reading;

Anemometer *anemometer_sensor;
AnemometerInfo anemometer_reading;

MQ6Sensor *mq6_sensor;
float mq6_ppm = 0;

WindDirectionSensor *wind_direction_sensor;
WindDirectionInfo wind_direction_reading;


unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  while(!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
  }
  time(&now);
  return now;
}


void setup_sensors() {
    dht_sensor = new DHTSensor(DHT_11_PIN, DHT_MODEL_DHT11);
    wind_direction_sensor = new WindDirectionSensor(WIND_DIRECTION_PIN);
    anemometer_sensor = new AnemometerSensor(ANEMOMETER_PIN)
    mq6_sensor = new MQ6Sensor();
}

void setup_network() {
    wifi_client = new Wifi(ssid, password);
    mqtt_client = new MQTT(wifi_client, broker_addr, broker_port, broker_user, broker_password, client_id);
}

void setup(){
    setup_sensors();
    setup_network();

    configTime(0, 0, ntpServer);
    delay(1000);

    Serial.begin(115200);
    Serial.print("Climate monitoring system started\n");
};

JsonDocument sensor_reading() {
    JsonDocument doc;
}

void read_sensors() {
    JsonObject dht_temp = doc["47f4bc3aa81749e9af06a408a5ee602d"].to<JsonObject>();
    JsonObject dht_hum = doc["a1cb7516c98e4339ab136f358aa7e38f"].to<JsonObject>();
    dht_reading = dht_sensor->read();
    dht_temp["value"] = dht_reading.temperature;
    dht_temp["timestamp"] = getTime();
    dht_hum["value"] = dht_reading.humidity;
    dht_hum["timestamp"] = dht_temp["timestamp"];

    JsonObject wind_dir = doc["4f531ada394e46a498f638d8bd45a3a3"].to<JsonObject>();
    wind_direction_reading = wind_direction_sensor->read();
    wind_dir["value"] = wind_direction_reading.direction; 
    wind_dir["timestamp"] = getTime();

    JsonObject anemometer_data = doc["39400ce1-b2c6-488f-a4d3-c27ae125b3bd"].to<JsonObject>();
    anemometer_reading = anemometer_sensor->read();
    anemometer_data["value"] = anemometer_reading.wind_speed;
    anemometer_data["timestamp"] = getTime();

    mq6_ppm = mq6_sensor->read_ppm();
    JsonObject mq_6_data = doc["54e41d13-b345-4c08-865a-6b69f363baed"].to<JsonObject>();
    mq_6_data["timestamp"] = timestamp;
    mq_6_data["value"] = ppm; 
    doc.shrinkToFit();
}

void loop() {
    read_sensors();
    // doc.printTo(Serial);
    mqtt_client->publish(mqtt_topic, doc);
}
