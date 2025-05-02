from django.db import models
from django.contrib.postgres.fields import HStoreField


class Sensor(models.Model):
    name = models.CharField(max_length=64)
    # device = models.ForeignKey(to=Device, on_delete=models.CASCADE)
    unit = models.CharField(max_length=32)
    measuring_type = models.CharField(
        max_length=8,
        choices=(("str", "String"), ("int", "Integer"), ("float", "Float")),
        default="string"
    )
    description = models.CharField(max_length=256, blank=True)
    is_active = models.BooleanField(default=True)
    additional_info = HStoreField()
    upper_limit = models.FloatField(null=True)
    lower_limit = models.FloatField(null=True)

    def __str__(self):
        return f"{self.name} ({self.unit}) <{self.id}>"


class SensorReading(models.Model):
    timestamp = models.DateTimeField(primary_key=True)
    sensor_id = models.PositiveIntegerField()
    value = models.CharField(max_length=64)

    class Meta:
        db_table = 'sensor_reading'
