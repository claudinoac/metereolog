from django.db import models
from uuid import uuid4

from iam.models import Organization


class Device(models.Model):
    name = models.CharField(max_length=255)
    model = models.CharField(max_length=255, blank=True, null=True)
    brand = models.CharField(max_length=255, blank=True, null=True)
    serial_number = models.CharField(max_length=255, blank=True, null=True)
    identifier = models.UUIDField(default=uuid4, unique=True)
    organization = models.ForeignKey(to=Organization, on_delete=models.CASCADE)
    is_active = models.BooleanField(default=True)
    mqtt_user = models.CharField(max_length=64)
    mqtt_password = models.CharField(max_length=64)

    def __str__(self):
        return f"{self.name} <{self.organization.identifier.hex}.{self.identifier.hex}>"

    @property
    def mqtt_topic(self):
        return f"mqtt.{self.organization.identifier.hex}.{self.identifier.hex}"
