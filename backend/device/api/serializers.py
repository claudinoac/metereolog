from rest_framework import serializers
from device.models import Device


class DeviceReadSerializer(serializers.ModelSerializer):
    organization = serializers.SerializerMethodField(read_only=True)

    class Meta:
        model = Device
        fields = [
            "name",
            "identifier",
            "organization",
            "is_active",
            "mqtt_user",
            "mqtt_password",
        ]

    def get_organization(self, instance):
        return instance.organization.name


class DeviceSerializer(DeviceReadSerializer):
    sensors = serializers.SerializerMethodField(read_only=True)

    class Meta:
        model = Device
        fields = [
            "name",
            "identifier",
            "organization",
            "is_active",
            "sensors",
        ]

    def get_sensors(self, instance):
        return instance.sensor_set.count()
