FROM python:3.10

LABEL maintainer="phithon <root@leavesongs.com>"

COPY requirements.txt /tmp/requirements.txt
RUN set -ex \
    && pip install -r /tmp/requirements.txt

ENV PGADMIN_SETUP_EMAIL=vulhub@example.com PGADMIN_SETUP_PASSWORD=vulhub
COPY config_local.py /usr/local/lib/python3.10/site-packages/pgadmin4/config_local.py

WORKDIR /usr/local/lib/python3.10/site-packages/pgadmin4
CMD [ "pgadmin4" ]
