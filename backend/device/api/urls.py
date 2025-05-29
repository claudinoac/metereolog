from device.api import views
from sensor.api.views import SensorListView
from django.urls import path

urlpatterns = [
    path("/<uuid:device_uid>", views.DeviceViewSet.as_view({
        "get": "retrieve",
    })),
    path("s", views.DeviceListView.as_view()),
    path("/<uuid:device_uid>/sensors", SensorListView.as_view()),
]
