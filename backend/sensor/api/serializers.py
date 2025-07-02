from sensor.models import SensorReading, Sensor
from rest_framework import serializers


class SensorReadingSerializer(serializers.ModelSerializer):
    timestamp = serializers.SerializerMethodField(read_only=True)

    class Meta:
        model = SensorReading
        fields = [
            "timestamp",
            "value"
        ]

    def get_timestamp(self, instance):
        if isinstance(instance, dict):
            return instance["bucket"]
        return instance.timestamp


class SensorSerializer(serializers.ModelSerializer):
    class Meta:
        model = Sensor
        fields = [
            "name",
            "unit",
            "identifier",
            "measuring_type",
            "description",
            "is_active",
            "additional_info",
            "upper_limit",
            "lower_limit",
        ]


class SensorCreateSerializer(serializers.ModelSerializer):
    class Meta:
        model = Sensor
        fields = [
            "name",
            "unit",
            "identifier",
            "measuring_type",
            "description",
            "is_active",
            "additional_info",
            "upper_limit",
            "lower_limit",
            "device",
        ]


