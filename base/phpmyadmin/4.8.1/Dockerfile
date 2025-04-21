FROM php:7.2-apache

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends unzip \
    && rm -r /var/lib/apt/lists/*

ARG SOURCE_URL=https://files.phpmyadmin.net/phpMyAdmin/4.8.1/phpMyAdmin-4.8.1-all-languages.zip

RUN set -ex \
    && cd /tmp \
    && curl -#SL ${SOURCE_URL} -o /tmp/phpmyadmin.zip \
    && unzip -q /tmp/phpmyadmin.zip \
    && cp -r phpMyAdmin-*/* /var/www/html/ \
    && docker-php-ext-install mbstring mysqli \
    && rm -rf /tmp/*