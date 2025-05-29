from rest_framework.viewsets import ViewSet
from rest_framework.permissions import IsAuthenticated
from rest_framework.response import Response
from rest_framework import status
from device.models import Device
from metereolog.base_views import BaseListView
from device.api.serializers import (
    DeviceSerializer, DeviceReadSerializer
)


class DeviceListView(BaseListView):
    serializer_class = DeviceSerializer
    model_class = Device
    filterable_params = ["name", "identifier", "organization__name"]


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
