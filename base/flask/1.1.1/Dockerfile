FROM python:3.6

LABEL maintainer="phithon <root@leavesongs.com>"

RUN mkdir /app \
    && pip install "jinja2==2.10.3" "flask==1.1.1" "gunicorn==20.0.0"

EXPOSE 8000

WORKDIR /app

CMD gunicorn -w 4 -b :8000 -u www-data -g www-data --access-logfile - app:app