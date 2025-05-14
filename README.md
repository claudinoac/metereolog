# 🌦️ Metereolog
Metereolog is a modular, full-stack platform for aggregating, storing, and visualizing weather data from distributed sensor stations.

It is designed to be easily deployed on low-power embedded devices and scalable across networks using a backend API, messaging infrastructure, and a responsive web dashboard.

## 📚 Overview
The project is composed of three main components:

* **Firmware:** Runs on weather station hardware (e.g. ESP32), collects sensor data (temperature, humidity, etc.), and transmits it over Wi-Fi, powered by PlatformIO.

* **Backend:** A REST API and MQTT --> AMQP processing service that receives data from remote sensors, stores it, and exposes it for clients, powered by Python, Django, Kombu and RabbitMQ.

* **Frontend:** A modern web interface for visualizing live and historical environmental data in real time powered by Vue.JS and Quasar..

## 🧩 Features
* 🛰️ Collects data from remote sensors (ex. DHT, BMP180)

* 🪝 Publishes and consumes data via MQTT and/or HTTP

* 📦 Dockerized backend for ease of deployment

* 📈 Responsive frontend dashboard for live monitoring

* ⚙️  Built-in support for RabbitMQ messaging

* 🧪 Easily extendable for new sensors or data endpoints

## Architecture Overview
![Architecture](https://github.com/claudinoac/metereolog/blob/main/docs/architecture.png?raw=true)

