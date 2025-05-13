from rest_framework.views import APIView
from rest_framework.permissions import IsAuthenticated
from rest_framework.response import Response
from rest_framework import serializers
from django.shortcuts import Http404
from sensor.models import Sensor, SensorReading
from django.utils import timezone
from datetime import timedelta
from django.db.models import Count, Avg, FloatField
from django.db.models.functions import Cast


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


class SensorChartView(APIView):
    permission_classes = [IsAuthenticated]

    def get(self, request, sensor_uid):
        sensor = Sensor.objects.filter(identifier=sensor_uid, is_active=True).first()
        if not sensor:
            raise Http404
        time_diff = int(request.query_params.get("interval_sec", 60))
        start_time = timezone.now() - timedelta(seconds=time_diff)
        rows = SensorReading.objects.filter(
            sensor_uid=sensor_uid, timestamp__gte=start_time
        )
        if sensor.measuring_type != 'str':
            rows = rows.time_bucket('timestamp', '1 minute').annotate(
                value=Cast('value', output_field=FloatField())
            ).annotate(Avg('value'))
        serializer = SensorReadingSerializer(rows, many=True)
        return Response(
            data=serializer.data
        )
