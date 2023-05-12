FROM vulhub/php:5.3-apache

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends unzip \
    && rm -r /var/lib/apt/lists/*

ARG SOURCE_URL=https://files.phpmyadmin.net/phpMyAdmin/4.4.15.6/phpMyAdmin-4.4.15.6-all-languages.zip

RUN set -ex \
    && cd /tmp \
    && curl -#SL ${SOURCE_URL} -o /tmp/phpmyadmin.zip \
    && unzip -q /tmp/phpmyadmin.zip \
    && cp -r phpMyAdmin-*/* /var/www/html/ \
    && rm -rf /tmp/*