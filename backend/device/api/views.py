from rest_framework.viewsets import ViewSet
from rest_framework.permissions import IsAuthenticated
from rest_framework.response import Response
from rest_framework import status, serializers
from device.models import Device


class DeviceSerializer(serializers.ModelSerializer):
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


class DeviceViewSet(ViewSet):
    permission_classes = [IsAuthenticated]
    serializer_class = DeviceSerializer

    def retrieve(self, request, device_uid):
        if device := Device.objects.filter(identifier=device_uid).first():
            serializer = self.serializer_class(instance=device)
            return Response(data=serializer.data)
        return Response(status=status.HTTP_404_NOT_FOUND)
