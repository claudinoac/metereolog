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

#define GPIO_PIN_4 (gpio_num_t) 4

DHTSensor *dht_sensor;
MQTT *mqtt_client;
DHTInfo dht_reading;
Wifi *wifi_client;

char* ssid = NULL;
char* password = NULL;
char *broker_addr = "192.168.0.11";
int broker_port = 1883;
char *broker_user = "metereolog";
char *broker_password = "metereolog";
char *client_id = "esp32_met_cit";

void setup(){
    dht_sensor = new DHTSensor(GPIO_PIN_4, DHT_MODEL_DHT11);
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
    wifi_client = new Wifi(ssid, password);
    mqtt_client = new MQTT(wifi_client, broker_addr, broker_port, broker_user, broker_password, client_id);
};

void loop(){
    dht_reading = dht_sensor->read();
    Serial.print("\nTemperature: ");
    Serial.print(dht_reading.temperature);
    Serial.print(" °C");
    Serial.print("\nHumidity: ");
    Serial.print(dht_reading.humidity);
    Serial.print(" %");
    JsonDocument vals;
    JsonDocument dht;
    dht["temperature"] = dht_reading.temperature;
    dht["humidity"] = dht_reading.humidity;
    vals["dht"] = dht;
    mqtt_client->publish("mqtt.user.arduino", vals);
    delay(1000);
};
