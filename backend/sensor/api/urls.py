from sensor.api import views
from django.urls import path


urlpatterns = [
    path("<uuid:sensor_uid>/chart", views.SensorChartView.as_view()),
]
