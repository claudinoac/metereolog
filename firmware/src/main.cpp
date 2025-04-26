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
#include "WindDirectionSensor.hpp"

#define GPIO_PIN_4 (gpio_num_t) 4
#define WIIND_DIRECTION_PIN (gpio_num_t) 32
#define ANEMOMETER_PIN (gpio_num_t) 35

#include "Anemometer.hpp"
#include "MQSensor.hpp"

#define GPIO_PIN_4 (gpio_num_t) 4
#define GPIO_PIN_2 (gpio_num_t) 2


DHTSensor *dht_sensor;
Anemometer *anemometer_sensor;
WindInfo anemometer_reading;
MQ6Sensor *mq6_sensor;
DHTInfo dht_reading;
MQTT *mqtt_client;
Wifi *wifi_client;

char* ssid = "Redmi Note 13";
char* password = "Pinto1995";
char *broker_addr = "192.168.0.11";
int broker_port = 1883;
char *broker_user = "metereolog";
char *broker_password = "metereolog";
char *client_id = "esp32_met_cit";
char *mqtt_topic = "mqtt.metereolog.esp32MetCit";

WindDirectionSensor *wind_direction_sensor;
WindDirectionInfo wind_direction_reading;

StaticJsonDocument<256> doc;


float Ro = 0.0f;
float RL = 4700.0f;

void setup(){
    dht_sensor = new DHTSensor(GPIO_PIN_4, DHT_MODEL_DHT11);
    wind_direction_sensor = new WindDirectionSensor(WIIND_DIRECTION_PIN);
    anemometer_sensor = new Anemometer(GPIO_PIN_2);
    mq6_sensor = new MQ6Sensor(ADC1_CHANNEL_6);
    pinMode(GPIO_PIN_2, INPUT_PULLUP);
    Ro = mq6_sensor->calibrate(RL);
    delay(1000);

    Serial.begin(115200);
    while (!Serial.available()){};
    Serial.print("\n\nEnter wifi SSID: ");
    while (!Serial.available()){};
    // ssid = strdup(Serial.readStringUntil('\n').c_str());
    // ssid[strlen(ssid) - 1] = '\0';
    // Serial.print("SSID is -----");
    // Serial.print(ssid);
    // Serial.print("-----\n\n\n");
    // Serial.print("\nEnter wifi Password: ");
    // while (!Serial.available()){};
    // password = strdup(Serial.readStringUntil('\n').c_str());
    // password[strlen(password) - 1] = '\0';
    // Serial.print("Password is -----");
    // Serial.print(password);
    // Serial.print("-----\n\n\n");
    Serial.print("\nEnter broker address: ");
    while (!Serial.available()){};
    broker_addr = strdup(Serial.readStringUntil('\n').c_str());
    Serial.print("Broker address is: -----");
    Serial.print(broker_addr);
    Serial.print("-----\n\n\n");
    wifi_client = new Wifi(ssid, password);
    mqtt_client = new MQTT(wifi_client, broker_addr, broker_port, broker_user, broker_password, client_id);
    Serial.print("Climate monitoring system started\n");
};

void loop() {
    dht_reading = dht_sensor->read();
    WindDirectionInfo wind_direction_reading = wind_direction_sensor->read();
    anemometer_reading = anemometer_sensor->read();
    float ppm = mq6_sensor->readPPM(Ro, RL);

    Serial.print("\nTemperature: ");
    Serial.print(dht_reading.temperature);
    Serial.print("\nHumidity: ");
    Serial.print(dht_reading.humidity);
    Serial.print(" %");
    Serial.print("\n Wind Direction: ");
    Serial.print(wind_direction_reading.direction);
    Serial.print(" (");
    Serial.print(wind_direction_reading.angle);
    Serial.print("º)");
    Serial.print(" Analog Voltage: ");
    Serial.print(wind_direction_reading.voltage);
    Serial.print("V\n");
    Serial.print("\nSpeed: ");
    Serial.print(anemometer_reading.windSpeed);
    Serial.print(" km/h");
    Serial.print("\nRPM: ");
    Serial.println(anemometer_reading.rotations);
    Serial.print("PPM: ");
    Serial.println(ppm);

    Serial.print("\n Publishing results to MQTT topic ");
    Serial.print(mqtt_topic);

    JsonObject dht = doc.createNestedObject("dht");
    JsonObject wind_sensor = doc.createNestedObject("wind_sensor");
    dht["temperature"] = dht_reading.temperature;
    dht["humidity"] = dht_reading.humidity;
    wind_sensor["direction"] = wind_direction_reading.direction;
    wind_sensor["voltage"] = wind_direction_reading.voltage;
    wind_sensor["speed"] = anemometer_reading.windSpeed;
    wind_sensor["rpm"] = anemometer_reading.rotations;
    wind_sensor["angle"] = wind_direction_reading.angle;
    doc["mq-6"]["glp"] = ppm;


    mqtt_client->publish(mqtt_topic, doc);

    Serial.print("\n\nWaiting 1 second for new reading...\n");
    delay(1000);
}
