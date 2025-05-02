from django.contrib import admin

from sensor.models import Sensor, SensorReading


class SensorAdmin(admin.ModelAdmin):
    pass


admin.site.register(Sensor, SensorAdmin)
admin.site.register(SensorReading)
