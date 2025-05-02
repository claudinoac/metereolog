from django.contrib import admin
from device.models import Device


class DeviceAdmin(admin.ModelAdmin):
    pass


admin.site.register(Device, DeviceAdmin)
