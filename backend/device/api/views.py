from rest_framework.viewsets import ViewSet
from rest_framework.permissions import IsAuthenticated
from rest_framework.response import Response
from rest_framework import status
from device.models import Device
from metereolog.base_views import BaseListView
from device.api.serializers import (
    DeviceSerializer, DeviceReadSerializer
)
from message_hub.services import rabbitmq_service
from django.db import transaction


class DeviceListView(BaseListView):
    serializer_class = DeviceSerializer
    model_class = Device
    filterable_params = ["name", "identifier", "organization__name"]

    def create(self, request):
        serializer = self.serializer_class(data=request.data)
        if not serializer.is_valid():
            return Response(data=serializer.errors, status=status.HTTP_400_BAD_REQUEST)
        with transaction.atomic():
            device = Device.objects.create(
                **serializer.validated_data, organization=request.user.organization, is_active=True
            )
            mqtt_infos = rabbitmq_service.create_user(device.identifier.hex)
            rabbitmq_service.configure_user_permissions(device.identifier.hex, device.mqtt_topic)
            device.mqtt_user, device.mqtt_password = mqtt_infos["username"], mqtt_infos["password"]
            device.save()
        return Response(data={"detail": f"Device {device.name} created successfully"}, status=status.HTTP_201_CREATED)


class DeviceViewSet(ViewSet):
    permission_classes = [IsAuthenticated]
    serializer_class = DeviceReadSerializer

    def retrieve(self, request, device_uid):
        if device := Device.objects.filter(identifier=device_uid).first():
            serializer = self.serializer_class(instance=device)
            return Response(data=serializer.data)
        return Response(status=status.HTTP_404_NOT_FOUND)

    def delete(self, request, device_uid):
        if device := Device.objects.filter(identifier=device_uid).first():
            device.delete()
        return Response(status=status.HTTP_204_NO_CONTENT)
