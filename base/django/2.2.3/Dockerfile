FROM python:3.6

LABEL maintainer="phithon <root@leavesongs.com>"

ADD https://raw.githubusercontent.com/vishnubob/wait-for-it/master/wait-for-it.sh /usr/local/bin/wait-for-it.sh

RUN set -ex \
    && pip install -U pip \
    && pip install "django==2.2.3" "psycopg2==2.8.3" \
    && chmod +x /usr/local/bin/wait-for-it.sh
