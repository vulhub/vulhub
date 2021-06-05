FROM python:3.5

LABEL maintainer="phithon <root@leavesongs.com>"

ADD https://raw.githubusercontent.com/vishnubob/wait-for-it/master/wait-for-it.sh /bin/wait-for-it.sh

COPY docker-entrypoint.sh /docker-entrypoint.sh

RUN mkdir /app \
    && pip install -U "django==1.11.4" "dj-database-url==0.5.0" "psycopg2==2.8.5" \
    && chmod +x /docker-entrypoint.sh /bin/wait-for-it.sh

EXPOSE 8000

WORKDIR /app

CMD ["/docker-entrypoint.sh"]