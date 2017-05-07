FROM vulhub/python:3.5

MAINTAINER phithon <root@leavesongs.com>

ADD requirements.txt /tmp/requirements.txt

RUN mkdir /app \
    && pip install -U -r /tmp/requirements.txt

EXPOSE 8000

WORKDIR /app

ENTRYPOINT ["gunicorn"]