from django.db import models
from django.contrib.postgres.fields import HStoreField


class Sensor(models.Model):
    name = models.CharField(max_length=64)
    # sensor = models.ForeignKey(to=Device, on_delete=models.CASCADE)
    unit = models.CharField(max_length=32)
    description = models.CharField(max_length=256, blank=True)
    is_active = models.BooleanField(default=True)
    additional_info = HStoreField()
    upper_limit = models.FloatField(null=True)
    lower_limit = models.FloatField(null=True)

    def __str__(self):
        return f"{self.name} ({self.unit}) <{self.id}>"
