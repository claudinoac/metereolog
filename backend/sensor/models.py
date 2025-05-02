from django.db import models
from django.contrib.postgres.fields import HStoreField
from uuid import uuid4
from device.models import Device


class Sensor(models.Model):
    name = models.CharField(max_length=64)
    device = models.ForeignKey(to=Device, on_delete=models.CASCADE, null=True)
    unit = models.CharField(max_length=32)
    identifier = models.UUIDField(default=uuid4, unique=True)
    measuring_type = models.CharField(
        max_length=8,
        choices=(("str", "String"), ("int", "Integer"), ("float", "Float")),
        default="string"
    )
    description = models.CharField(max_length=256, blank=True)
    is_active = models.BooleanField(default=True)
    additional_info = HStoreField(blank=True, null=True)
    upper_limit = models.FloatField(null=True)
    lower_limit = models.FloatField(null=True)

    def __str__(self):
        return f"{self.name} ({self.unit}) <{self.id}>"


class SensorReading(models.Model):
    timestamp = models.DateTimeField(primary_key=True)
    sensor_uid = models.UUIDField()
    value = models.CharField(max_length=64)

    class Meta:
        db_table = 'sensor_reading'
        unique_together = (('timestamp', 'sensor_uid'),)
