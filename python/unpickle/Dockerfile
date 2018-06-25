FROM python:latest

MAINTAINER phithon <root@leavesongs.com>

ADD requirements.txt /tmp/requirements.txt

RUN mkdir /app \
    && pip install -i  https://pypi.doubanio.com/simple/  --trusted-host pypi.doubanio.com -U -r /tmp/requirements.txt

EXPOSE 8000

WORKDIR /app

ENTRYPOINT ["gunicorn"]