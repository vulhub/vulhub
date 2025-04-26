FROM debian:jessie

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && apt-get update \
    && apt-get install -y wget aria2 \
    && apt-get install -y --no-install-recommends cron \
    && mkdir -p /usr/aria2/data/

COPY aria2.conf /usr/aria2/aria2.conf

CMD set -ex \
    && cron \
    && aria2c --conf-path=/usr/aria2/aria2.conf