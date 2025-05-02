from django.contrib import admin

from sensor.models import Sensor


class SensorAdmin(admin.ModelAdmin):
    pass


admin.site.register(Sensor, SensorAdmin)
