FROM python:3.10

ADD https://raw.githubusercontent.com/vishnubob/wait-for-it/master/wait-for-it.sh /usr/local/bin/wait-for-it.sh

RUN set -ex \
    && pip install -U pip \
    && pip install "django==4.0.5" "psycopg2_binary==2.9.3" \
    && chmod +x /usr/local/bin/wait-for-it.sh
