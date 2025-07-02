from django.conf import settings
import string
import random
import os
import requests


class RabbitMQService:
    endpoints = {
        "user": "/api/users/{}",
        "permissions": "/api/permissions/%2F/{}",
        "topic_permissions": "/api/topic-permissions/%2F/{}",
    }

    def create_user(self, username):
        password = self._generate_password()
        endpoint = self.endpoints["user"].format(username)
        response = self._make_request(endpoint, {"password": password, "tags": ["customer"]}, "put")
        response.raise_for_status()
        return {
            "username": username,
            "password": password
        }

    def configure_user_permissions(self, username, topic):
        permissions = {
            "configure": f"^amq\\.topic$|^mqtt-subscription-{username}.*$",
            "write": f"^amq\\.topic$|^mqtt-subscription-{username}.*$",
            "read": f"^amq\\.topic$|^mqtt-subscription-{username}.*$"
        }
        perm_endpoint = self.endpoints["permissions"].format(username)
        perm_response = self._make_request(perm_endpoint, permissions, "put")
        perm_response.raise_for_status()

        topic_permissions = {
            "exchange": "amq.topic",
            "write": f"^mqtt-subscription-{username}.*$|^{topic}$",
            "read": f"^mqtt-subscription-{username}.*$|^{topic}$",
        }
        topic_endpoint = self.endpoints["topic_permissions"].format(username)
        topic_response = self._make_request(topic_endpoint, topic_permissions, "put")
        topic_response.raise_for_status()

    def _make_request(self, endpoint, data=None, method="get"):
        http_scheme = "https" if settings.RMQ_SSL else "http"
        return getattr(requests, method)(
            f"{http_scheme}://{settings.RMQ_ADDR}:{settings.RMQ_API_PORT}/{endpoint}",
            json=data,
            auth=(settings.RMQ_USER, settings.RMQ_PASSWORD),
            headers={"Content-Type": "application/json"}
        )

    def _generate_password(self):
        chars = string.ascii_letters + string.digits
        random.seed = (os.urandom(1024))
        return ''.join(random.choice(chars) for i in range(20))


rabbitmq_service = RabbitMQService()
