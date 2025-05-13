from kombu import Connection, Consumer, Queue, Exchange
from django.conf import settings
import logging
import socket
from json import loads as json_loads, dumps as json_dumps, JSONDecodeError
from sensor.handlers import SensorReadingHandler
from pprint import PrettyPrinter
from django.db import connection


logger = logging.getLogger(__name__)
pp = PrettyPrinter()


class MessageHubListener:
    queue = None
    consumer = None

    def __init__(self):
        self.queue = Queue('sensors', Exchange('mqtt.*.*', type='topic'), routing_key="mqtt.*.*")
        self.reading_handler = SensorReadingHandler()

    def run(self):
        logger.info("Starting message hub listener..")
        self.connection = Connection(settings.BROKER_URL, heartbeat=10)
        logger.info("Connected to AMQP Broker!")
        self.consumer = Consumer(self.connection, [self.queue], accept=['json'])
        self.consumer.register_callback(self.handle_message)
        while(True):
            try:
                self.consume()
            except ConnectionResetError as exception:
                logger.exception(f"Connection Reset while trying to consume queue: {exception}")

    def reconnect(self):
        revived_connection = self.connection.clone()
        revived_connection.ensure_connection(max_retries=3)
        channel = revived_connection.channel()
        self.consumer.revive(channel)
        self.consumer.consume()
        self.connection = revived_connection

    def consume(self):
        self.reconnect()
        while True:
            try:
                self.connection.drain_events(timeout=1)
            except socket.timeout:
                self.connection.heartbeat_check()
            except Exception as exception:
                logger.exception(f"Error while trying to consume queue: {exception}")

    def handle_message(self, body, message):
        try:
            routing_key = message.delivery_info["routing_key"]
            _, org_uid, device_uid = routing_key.split(".")
            payload = json_loads(body)
            self.reading_handler.handle_readings(org_uid, device_uid, payload)
        except JSONDecodeError:
            logger.error("Invalid message: {body}")
        message.ack()
