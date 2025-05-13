ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
$(eval $(ARGS):;@:)
MAKEFLAGS += --silent
$(eval TEST_TYPE := $(shell [[ -z "$(ARGS)" ]] && echo "null" || echo "$(ARGS)"))
RUN = docker compose run --rm -w /code api
EXEC = docker compose exec -w /code api
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
	@ docker-compose up -d
	@ make migrations
	@ make migrate
	@ $(EXEC) python3 manage.py createsuperuser

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
run-detached:
	@ docker compose up -d --no-recreate

.PHONY: build
build:  ### Build local stack
	@ docker compose build

.PHONY: stop
stop:  ### Stop the application
	@ docker compose stop

.PHONY: build-firmware
build-firmware:  ### builds the firmware binary
	@ pio run -d firmware

.PHONY: flash-firmware
flash-firmware:  ### Flashes the firmware binary into the board
	@ pio run --target upload -d firmware
