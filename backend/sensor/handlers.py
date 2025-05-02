import logging
from datetime import datetime

from sensor.models import Sensor, SensorReading
from device.models import Device


logger = logging.getLogger(__name__)


class SensorReadingHandler:
    def __init__(self):
        pass

    def handle_readings(self, org_uid, device_uid, payload):
        if not Device.objects.filter(
            identifier=device_uid, is_active=True,
            organization__identifier=org_uid, organization__is_active=True
        ).exists():
            logger.info(f"Device {device_uid} not found or not active.")
            return

        if not isinstance(payload, dict):
            logger.error(f"Error: payload from {org_uid}::{device_uid} is in incorrect format: {payload}")
            return

        for sensor_uid, sensor_data in payload.items():
            if not Sensor.objects.filter(identifier=sensor_uid):
                logger.error(f"Sensor with uid {sensor_uid} not found or is not active")
                return

            try:
                timestamp = datetime.utcfromtimestamp(sensor_data.get("timestamp"))
            except Exception:
                logger.warning(f"Sensor {sensor_uid} returned invalid timestamp")
            else:
                if sensor_value := sensor_data.get("value"):
                    return SensorReading.objects.create(timestamp=timestamp, value=sensor_value, sensor_uid=sensor_uid)
                else:
                    logger.warning(f"Sensor {sensor_uid} returned no valid value.")
