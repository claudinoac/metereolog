from rest_framework.views import APIView
from rest_framework.permissions import IsAuthenticated
from rest_framework.response import Response
from rest_framework.viewsets import ViewSet
from django.shortcuts import Http404
from sensor.models import Sensor, SensorReading
from django.utils import timezone
from datetime import timedelta
from django.db.models import Avg, FloatField
from django.db.models.functions import Cast
from sensor.api.serializers import SensorReadingSerializer, SensorSerializer, SensorCreateSerializer
from metereolog.base_views import BaseListView
from datetime import datetime
from rest_framework import status
from device.models import Device


class SensorChartView(APIView):
    permission_classes = [IsAuthenticated]
    time_buckets = {
        "second": "1 second",
        "minute": "1 minute",
        "hour": "1 hour",
        "day": "1 day",
        "week": "1 week",
        "month": "1 month",
        "year": "1 year",
    }

    def get(self, request, sensor_uid):
        sensor = Sensor.objects.filter(
            identifier=sensor_uid, device__organization=request.user.organization, is_active=True
        ).first()
        if not sensor:
            raise Http404
        current_time = timezone.now()
        if start := request.query_params.get("start"):
            start_time = datetime.fromisoformat(start.replace("Z", "+00:00"))
        else:
            start_time = current_time - timedelta(seconds=900)
        if end := request.query_params.get("end"):
            end_time = datetime.fromisoformat(end.replace("Z", "+00:00"))
        else:
            end_time = current_time
        rows = SensorReading.objects.filter(
            sensor_uid=sensor_uid, timestamp__gte=start_time, timestamp__lte=end_time
        )
        if sensor.measuring_type != 'str':
            delta = end_time - start_time
            interval = self.time_buckets["second"]
            if delta > timedelta(minutes=30):
                interval = self.time_buckets["minute"]
            if delta > timedelta(hours=3):
                interval = self.time_buckets["hour"]
            if delta > timedelta(days=3):
                interval = self.time_buckets["day"]
            if delta > timedelta(weeks=3):
                interval = self.time_buckets["week"]
            if delta > timedelta(days=90):
                interval = self.time_buckets["month"]
            if delta > timedelta(days=730):
                interval = self.time_buckets["year"]

            rows = rows.time_bucket('timestamp', interval).annotate(
                value=Cast('value', output_field=FloatField())
            ).annotate(Avg('value'))
        serializer = SensorReadingSerializer(rows, many=True)
        return Response(
            data=serializer.data
        )


class SensorListView(BaseListView):
    serializer_class = SensorSerializer
    write_serializer_class = SensorCreateSerializer
    model_class = Sensor
    filterable_params = ["name", "identifier", "description", "additional_info", "unit"]

    def get_base_queryset(self):
        return self.model_class.objects.filter(device__identifier=self.kwargs.get("device_uid"))

    def post(self, request, device_uid):
        device = Device.objects.filter(identifier=device_uid, organization=request.user.organization).first()
        if not device:
            return Response(status=status.HTTP_404_NOT_FOUND)
        serializer = self.serializer_class(data=request.data)
        if not serializer.is_valid():
            return Response(status=status.HTTP_400_BAD_REQUEST, data=serializer.errors)
        sensor = Sensor.objects.create(device=device, **serializer.validated_data)
        return Response(status=status.HTTP_201_CREATED, data={"detail": f"Sensor {sensor.name} created successfully"})


class SensorViewSet(ViewSet):
    permission_classes = [IsAuthenticated]
    serializer_class = SensorSerializer

    def retrieve(self, request, device_uid, sensor_uid):
        if sensor := Sensor.objects.filter(identifier=sensor_uid, device__identifier=device_uid).first():
            serializer = self.serializer_class(instance=sensor)
            return Response(data=serializer.data)
        return Response(status=status.HTTP_404_NOT_FOUND)

    def delete(self, request, sensor_uid):
        if sensor := Sensor.objects.filter(
            identifier=sensor_uid, device__organization=request.user.organization
        ).first():
            sensor.delete()
        return Response(status=status.HTTP_204_NO_CONTENT)
