from pathlib import Path
from os import environ, path
from metereolog.settings.base import *  # noqa

BASE_DIR = Path(__file__).resolve().parent.parent


SECRET_KEY = environ.get("SECRET_KEY")

DEBUG = False

ALLOWED_HOSTS = ["app.metereolog.site"]
CSRF_TRUSTED_ORIGINS = ["https://app.metereolog.site"]

DATABASES = {
    'default': {
        'ENGINE': 'timescale.db.backends.postgresql',
        'NAME': environ.get("POSTGRES_DB", 'metereolog'),
        'USER': environ.get("POSTGRES_USER"),
        'PASSWORD': environ.get("POSTGRES_PASSWORD"),
        'HOST': environ.get("POSTGRES_HOST", "postgres"),
        'PORT': environ.get("POSTGRES_PORT", "5432"),
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

RMQ_USER = environ.get("RABBITMQ_DEFAULT_USER")
RMQ_PASSWORD = environ.get("RABBITMQ_DEFAULT_PASS")
RMQ_ADDR = environ.get("RABBITMQ_ADDR", "rabbitmq")
RMQ_AMQP_PORT = environ.get("RABBITMQ_AMQP_PORT", 5672)
RMQ_API_PORT = environ.get("RABBITMQ_API_PORT", 15672)
RMQ_SSL = False
BROKER_URL = f"amqp://{RMQ_USER}:{RMQ_PASSWORD}@{RMQ_ADDR}:{RMQ_AMQP_PORT}/"

STATIC_ROOT = path.join(path.dirname(BASE_DIR), "static")
MEDIA_ROOT = path.join(path.dirname(BASE_DIR), "media")
