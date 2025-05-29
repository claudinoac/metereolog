from rest_framework.views import APIView
from rest_framework.permissions import IsAuthenticated
from rest_framework.response import Response
from django.shortcuts import Http404
from sensor.models import Sensor, SensorReading
from django.utils import timezone
from datetime import timedelta
from django.db.models import Avg, FloatField
from django.db.models.functions import Cast
from sensor.api.serializers import SensorReadingSerializer
from metereolog.base_views import BaseListView
from rest_framework import serializers


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


class SensorListView(BaseListView):
    serializer_class = SensorSerializer
    model_class = Sensor
    filterable_params = ["name", "identifier", "description", "additional_info", "unit"]

    def get_base_queryset(self):
        return self.model_class.objects.filter(device__identifier=self.kwargs.get("device_uid"))
