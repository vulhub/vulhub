FROM php:5.6.38-apache

LABEL maintainer="phithon <root@leavesongs.com>"

ARG MIRROR_URL=https://github.com/vulhub/mirrors/raw/master/debian/stretch/aliyun.list

RUN set -ex \
    && curl -sL $MIRROR_URL -o /etc/apt/sources.list \
    && apt-get update \
    && apt-get install --no-install-recommends -y libc-client-dev libkrb5-dev ssh-client \
    && docker-php-ext-configure imap --with-kerberos --with-imap-ssl \
    && docker-php-ext-install imap \
    && rm -r /var/lib/apt/lists/*
