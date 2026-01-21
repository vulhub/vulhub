FROM python:3.13-slim

LABEL maintainer="Cycloctane <Cycloctane@octane.top>"

RUN pip install pgadmin4==9.1

COPY config_local.py /usr/local/lib/python3.13/site-packages/pgadmin4/config_local.py
COPY servers.json /tmp/servers.json

RUN PGADMIN_SETUP_EMAIL=vulhub@example.com PGADMIN_SETUP_PASSWORD=vulhub \
    python /usr/local/lib/python3.13/site-packages/pgadmin4/setup.py setup-db && \
    python /usr/local/lib/python3.13/site-packages/pgadmin4/setup.py load-servers --user vulhub@example.com /tmp/servers.json

CMD [ "pgadmin4" ]
