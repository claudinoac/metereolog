from pathlib import Path
from os import environ
from metereolog.settings.base import *  # noqa

BASE_DIR = Path(__file__).resolve().parent.parent


SECRET_KEY = environ.get("SECRET_KEY")

DEBUG = True

ALLOWED_HOSTS = []

DATABASES = {
    'default': {
        'ENGINE': 'timescale.db.backends.postgresql',
        'NAME': 'metereolog',
        'USER': 'metereolog',
        'PASSWORD': 'metereolog',
        'HOST': 'postgres',
        'PORT': '5432',
    }
}

LOGGING = {
    "version": 1,
    "disable_existing_loggers": False,
    "handlers": {
        "console": {
            "class": "logging.StreamHandler",
        },
    },
    "root": {
        "handlers": ["console"],
        "level": "INFO",
    },
}

AUTH_PASSWORD_VALIDATORS = [
    {
        "NAME": "django.contrib.auth.password_validation.UserAttributeSimilarityValidator",
    },
    {
        "NAME": "django.contrib.auth.password_validation.MinimumLengthValidator",
    },
    {
        "NAME": "django.contrib.auth.password_validation.CommonPasswordValidator",
    },
    {
        "NAME": "django.contrib.auth.password_validation.NumericPasswordValidator",
    },
]

RMQ_USER = "metereolog"
RMQ_PASSWORD = "metereolog"
RMQ_ADDR = "rabbitmq"
RMQ_AMQP_PORT = 5672
RMQ_API_PORT = 15672
RMQ_SSL = False
BROKER_URL = f"amqp://{RMQ_USER}:{RMQ_PASSWORD}@{RMQ_ADDR}:{RMQ_AMQP_PORT}/"
