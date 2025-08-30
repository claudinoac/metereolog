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
#include "AnemometerSensor.hpp"
#include "MQSensor.hpp"


#define WIND_DIRECTION_PIN (gpio_num_t) 32
#define ANEMOMETER_PIN (gpio_num_t) 2
#define DHT_11_PIN (gpio_num_t) 4



const char* ssid = ENV_WIFI_SSID;
const char* password = ENV_WIFI_PASS;

const char *broker_user = ENV_BROKER_USER;
const char *broker_password = ENV_BROKER_PASS;
const char *broker_addr = ENV_BROKER_ADDR;
const int broker_port = atoi(ENV_BROKER_PORT);

const char *client_id = ENV_BROKER_CLIENT_ID;
const char *mqtt_topic = ENV_BROKER_TOPIC;

const char* ntpServer = "pool.ntp.org";
unsigned long epochTime; 


MQTT *mqtt_client;
Wifi *wifi_client;

DHTSensor *dht_sensor;
DHTInfo dht_reading;

AnemometerSensor *anemometer_sensor;
AnemometerInfo anemometer_reading;

MQ6Sensor *mq6_sensor;
float mq6_ppm = 0;

WindDirectionSensor *wind_direction_sensor;
WindDirectionInfo wind_direction_reading;

JsonDocument doc;

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
    anemometer_sensor = new AnemometerSensor(ANEMOMETER_PIN);
    // mq6_sensor = new MQ6Sensor();
}

void setup_network() {
    wifi_client = new Wifi((char *)ssid, (char *)password);
    mqtt_client = new MQTT(wifi_client, (char *)broker_addr, broker_port, (char *)broker_user, (char *)broker_password, (char *)client_id);
}

void setup(){
    setup_sensors();
    setup_network();

    configTime(0, 0, ntpServer);
    delay(1000);

    Serial.begin(115200);
    Serial.print("Climate monitoring system started\n");
};

void read_sensors() {
    JsonObject dht_temp = doc["ed8121fb4e7e40d69604bace6c8bebe3"].to<JsonObject>();
    JsonObject dht_hum = doc["079ffc8fec6246ac9be601bfa8426cdd"].to<JsonObject>();
    dht_reading = dht_sensor->read();
    dht_temp["value"] = dht_reading.temperature;
    dht_temp["timestamp"] = getTime();
    dht_hum["value"] = dht_reading.humidity;
    dht_hum["timestamp"] = dht_temp["timestamp"];

    JsonObject wind_dir = doc["d443ad3dc38248ed8af4f2b05b9d6e16"].to<JsonObject>();
    wind_direction_reading = wind_direction_sensor->read();
    wind_dir["value"] = wind_direction_reading.direction; 
    wind_dir["timestamp"] = getTime();

    JsonObject anemometer_data = doc["15f27518074749829fcdb6634cdcd087"].to<JsonObject>();
    anemometer_reading = anemometer_sensor->read();
    anemometer_data["value"] = anemometer_reading.wind_speed;
    anemometer_data["timestamp"] = getTime();

    // mq6_ppm = mq6_sensor->read_ppm();
    // JsonObject mq_6_data = doc["54e41d13-b345-4c08-865a-6b69f363baed"].to<JsonObject>();
    // mq_6_data["timestamp"] = getTime();
    // mq_6_data["value"] = mq6_ppm; 

    doc.shrinkToFit();
}

void loop() {
    read_sensors();
    // doc.printTo(Serial);
    mqtt_client->publish((char *)mqtt_topic, doc);
}
