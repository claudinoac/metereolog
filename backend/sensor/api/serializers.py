from sensor.models import SensorReading
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
