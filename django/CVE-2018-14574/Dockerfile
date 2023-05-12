FROM python:3.7-alpine

LABEL maintainer="phithon <root@leavesongs.com>"

RUN pip install -U pip \
    && pip install django==2.0.7

WORKDIR /usr/src

CMD ["python", "app.py", "runserver", "0.0.0.0:8000"]