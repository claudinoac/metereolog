from django.core.management.base import BaseCommand
from time import sleep
import logging
from message_hub.listener import MessageHubListener


logger = logging.getLogger(__name__)


class Command(BaseCommand):
    help = "Runs message hub, the AMQP listener for MQTT messages"

    def handle(self, *args, **kwargs):
        listener = MessageHubListener()
        listener.run()
