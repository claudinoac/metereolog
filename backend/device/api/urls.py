from device.api import views
from django.urls import path

urlpatterns = [
    path("<uuid:device_uid>", views.DeviceViewSet.as_view({
        "get": "retrieve",
    })),
]
