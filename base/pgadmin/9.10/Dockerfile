FROM python:3.13-slim

LABEL maintainer="Cycloctane <Cycloctane@octane.top>"

RUN apt update && apt install -y --no-install-recommends postgresql-client-17 && apt clean

RUN pip install pgadmin4==9.10 -i https://mirrors.aliyun.com/pypi/simple

COPY config_local.py /usr/local/lib/python3.13/site-packages/pgadmin4/config_local.py
COPY servers.json /tmp/servers.json

RUN PGADMIN_SETUP_EMAIL=vulhub@example.com PGADMIN_SETUP_PASSWORD=vulhub \
    python /usr/local/lib/python3.13/site-packages/pgadmin4/setup.py setup-db && \
    python /usr/local/lib/python3.13/site-packages/pgadmin4/setup.py load-servers --user vulhub@example.com /tmp/servers.json

CMD [ "pgadmin4" ]
