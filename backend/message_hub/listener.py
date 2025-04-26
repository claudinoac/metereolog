from kombu import Connection, Consumer, Queue, Exchange
from django.conf import settings
import logging
import socket
from json import loads as json_loads, dumps as json_dumps, JSONDecodeError
from pprint import PrettyPrinter


logger = logging.getLogger(__name__)
pp = PrettyPrinter()


class MessageHubListener:
    queue = None
    consumer = None

    def __init__(self):
        self.queue = Queue('sensors', Exchange('mqtt.*.*', type='topic'), routing_key="mqtt.*.*")

    def run(self):
        logger.info("Starting message hub listener..")
        self.connection = Connection(settings.BROKER_URL, heartbeat=10)
        logger.info("Connected to AMQP Broker!")
        self.consumer = Consumer(self.connection, [self.queue], accept=['json'])
        self.consumer.register_callback(self.handle_message)
        while(True):
            try:
                self.consume()
            except ConnectionResetError:
                pass

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
            except Exception as exc:
                pass

    def handle_message(self, body, message):
        try:
            routing_key = message.delivery_info["routing_key"]
            _, user, device = routing_key.split(".")
            payload = json_loads(body)
            payload_display = json_dumps(payload, indent=4)
            logger.info(f"Payload received from device {device} using user {user}: {payload_display}")
        except JSONDecodeError:
            logger.error("Invalid message: {body}")
        message.ack()
