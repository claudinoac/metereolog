FROM python:3.10-slim-bullseye as build

ARG TARGETARCH
ARG POETRY_VERSION=1.7.1
SHELL ["/bin/bash", "-c"]

RUN apt-get update \
    && apt-get install --no-install-recommends curl default-libmysqlclient-dev build-essential libpq-dev -y \
    && rm -rf /var/lib/apt/lists/* \
    && pip install -U pip setuptools wheel poetry \
    && poetry config virtualenvs.create false

COPY pyproject.toml pyproject.toml
# COPY poetry.lock poetry.lock

RUN poetry install --no-cache --no-root --with $(test "$TARGETARCH" = arm64 && echo "arm64" || echo "amd64"),dev && rm -rf /root/.cache/*

WORKDIR /code
EXPOSE 8009
CMD ["python3", "manage.py", "runserver", "8009"] 
