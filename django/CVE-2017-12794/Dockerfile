FROM vulhub/python:3.5

MAINTAINER phithon <root@leavesongs.com>

ADD requirements.txt /tmp/requirements.txt

ADD https://raw.githubusercontent.com/vishnubob/wait-for-it/master/wait-for-it.sh /bin/wait-for-it.sh

ADD docker-entrypoint.sh /docker-entrypoint.sh

RUN mkdir /app \
    && pip install -U -r /tmp/requirements.txt \
    && chmod +x /docker-entrypoint.sh /bin/wait-for-it.sh

EXPOSE 8000

WORKDIR /app

CMD ["/docker-entrypoint.sh"]