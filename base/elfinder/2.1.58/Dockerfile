FROM php:7.4-apache

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends unzip zip

RUN set -ex \
    && cd /var/www/html/ \
    && curl -sSL https://github.com/Studio-42/elFinder/archive/2.1.58.tar.gz | tar --strip-components 1 -xz

RUN set -ex \
    && cd /var/www/html/ \
    && mv ./php/connector.minimal.php-dist ./php/connector.minimal.php \
    && cp elfinder.html index.html \
    && chown www-data:www-data -R ./
