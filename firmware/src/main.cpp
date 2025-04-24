#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <esp_log.h>
#include <sdkconfig.h>
#include <driver/gpio.h>
#include <Arduino.h>
#include "DHTSensor.hpp"
#include <ArduinoJson.h>
#include "wifi.hpp"
#include "mqtt.hpp"
#include "AnemometerSensor.hpp"
#include "WindDirectionSensor.hpp"

#define GPIO_PIN_4 (gpio_num_t) 4
#define WIIND_DIRECTION_PIN (gpio_num_t) 32
#define ANEMOMETER_PIN (gpio_num_t) 35

DHTSensor *dht_sensor;
DHTInfo dht_reading;
MQTT *mqtt_client;
Wifi *wifi_client;

char* ssid = NULL;
char* password = NULL;
char *broker_addr = "192.168.0.11";
int broker_port = 1883;
char *broker_user = "metereolog";
char *broker_password = "metereolog";
char *client_id = "esp32_met_cit";
char *mqtt_topic = "mqtt.metereolog.esp32_met_cit";

AnemometerSensor *anemometer_sensor;
AnemometerInfo anemometer_reading;
WindDirectionSensor *wind_direction_sensor;
WindDirectionInfo wind_direction_reading;

void setup(){
    dht_sensor = new DHTSensor(GPIO_PIN_4, DHT_MODEL_DHT11);
    anemometer_sensor = new AnemometerSensor(ANEMOMETER_PIN);
    wind_direction_sensor = new WindDirectionSensor(WIIND_DIRECTION_PIN);
    Serial.begin(115200);
    while (!Serial.available()){};
    Serial.print("\n\nEnter wifi SSID: ");
    while (!Serial.available()){};
    ssid = strdup(Serial.readStringUntil('\n').c_str());
    ssid[strlen(ssid) - 1] = '\0';
    Serial.print("SSID is -----");
    Serial.print(ssid);
    Serial.print("-----\n\n\n");
    Serial.print("\nEnter wifi Password: ");
    while (!Serial.available()){};
    password = strdup(Serial.readStringUntil('\n').c_str());
    password[strlen(password) - 1] = '\0';
    Serial.print("Password is -----");
    Serial.print(password);
    Serial.print("-----\n\n\n");
    Serial.print("\nEnter broker address: ")
    while (!Serial.available()){};
    broker_addr = strdup(Serial.readStringUntil('\n').c_str());
    Serial.print("Broker address is: -----");
    Serial.print(broker_addr);
    Serial.print("-----\n\n\n");
    wifi_client = new Wifi(ssid, password);
    mqtt_client = new MQTT(wifi_client, broker_addr, broker_port, broker_user, broker_password, client_id);
    Serial.print("Climate monitoring system started\n");
};

void loop(){
    // Reading DHT Temperature and Humidity Sensor
    dht_reading = dht_sensor->read();
    WindDirectionInfo wind_direction_reading = wind_direction_sensor->read();
    Serial.print("\nTemperature: ");
    Serial.print(dht_reading.temperature);
    Serial.print(" °C");
    Serial.print("\nHumidity: ");
    Serial.print(dht_reading.humidity);
    Serial.print(" %");
    Serial.print("\n Wind Direction: ");
    Serial.print(wind_direction_reading.direction.c_str());
    Serial.print(" (");
    Serial.print(wind_direction_reading.angle);
    Serial.print("º)");
    Serial.print(" Analog Voltage: ");
    Serial.print(wind_direction_reading.voltage);
    Serial.print("V")

    Serial.print("Publishing results to MQTT topic ")
    JsonDocument vals;
    JsonDocument dht;
    JsonDocument wind_sensor;

    wind_sensor["direction"] = wind_direction_reading.direction.c_str();
    wind_sensor["voltage"] = wind_direction_reading.voltage.c_str();
    wind_sensor["angle"] = wind_direction_reading.angle;
    dht["temperature"] = dht_reading.temperature;
    dht["humidity"] = dht_reading.humidity;
    vals["dht"] = dht;
    vals["wind_sensor"] = wind_sensor;
    mqtt_client->publish(mqtt_topic, vals);

    Serial.print("\n\nWaiting 1 second for new reading...\n");
    delay(1000);
};
