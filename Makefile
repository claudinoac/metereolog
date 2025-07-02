ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
$(eval $(ARGS):;@:)
MAKEFLAGS += --silent
$(eval TEST_TYPE := $(shell [[ -z "$(ARGS)" ]] && echo "null" || echo "$(ARGS)"))
RUN = docker compose -f docker-compose-dev.yml run --rm -w /code api
EXEC = docker compose -f docker-compose-dev.yml exec -w /code api
SHELL = /bin/bash
CURRENT_ARCH := $(shell /usr/bin/uname -m)
export CURRENT_ARCH

# HELP COMMANDS
.PHONY: help
help: ## show this help
	@echo 'usage: make [target] [option]'
	@echo ''
	@echo 'Available targets:'
	@egrep '^(.+)\:\ .*##\ (.+)' ${MAKEFILE_LIST} | sed 's/:.*##/#/' | column -t -c 2 -s '#'

.PHONY: init
init:  ### Install dependencies and start applications
	@ which docker || (echo "Please, install docker\n"\
	 " -> MacOS: https://docs.docker.com/docker-for-mac/install/ \n"\
	 " -> Ubuntu: https://docs.docker.com/install/linux/docker-ce/ubuntu/ \n"\
	 " -> Fedora: https://docs.docker.com/install/linux/docker-ce/fedora/ "; exit 1;)
	@ -docker network create metereolog
	@ docker compose -f docker-compose-dev.yml up -d
	@ echo -n 'Waiting for postgres to be ready...'
	@ while true; do echo -n '.'; docker compose -f docker-compose-dev.yml exec postgres pg_isready >> /dev/null && break; done
	@ make migrations
	@ make migrate
	@ $(EXEC) python manage.py loaddata metereolog/fixtures/initial_data.json
	@ $(EXEC) python3 manage.py createsuperuser

.PHONY: deploy
deploy:  ### Deploy application to production
	@ docker compose -f docker-compose-prod.yml up -d
	@ docker compose -f docker-compose-prod.yml exec api python manage.py migrate
	@ docker compose -f docker-compose-prod.yml exec api python manage.py collectstatic --noinput
	@ docker compose -f docker-compose-prod.yml restart api message-hub
	@ docker compose -f docker-compose-prod.yml exec ui npx quasar build
	@ docker compose -f docker-compose-prod.yml kill ui

.PHONY: migrations $(ARGS)
migrations: run-detached  ### Generate pending migrations
	@ $(EXEC) python3 manage.py makemigrations $(ARGS)

.PHONY: migrate
migrate: run-detached  ### Apply pending migrations
	@ $(EXEC) python3 manage.py migrate

.PHONY: superuser
superuser: run-detached  ### Create a superuser
	@ $(EXEC) python3 manage.py createsuperuser

.PHONY: shell
shell: run-detached  ### Opens the application shell
	@ $(EXEC) python3 manage.py shell_plus

.PHONY: bash
bash: run-detached  ### Opens the application shell
	@ $(EXEC) bash

.PHONY: run-detached
run-detached:  ### Runs all containers (backend and frontend)
	@ docker compose -f docker-compose-dev.yml up -d --no-recreate

.PHONY: build
build:  ### Build local stack
	@ docker compose -f docker-compose-dev.yml build

.PHONY: stop
stop:  ### Stop the application
	@ docker compose -f docker-compose-dev.yml stop

.PHONY: build-firmware
build-firmware:  ### builds the firmware binary
	@ pio run -d firmware

.PHONY: flash-firmware
flash-firmware:  ### Flashes the firmware binary into the board
	@ pio run --target upload -d firmware
