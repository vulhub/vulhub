FROM php:7.2.15-apache

LABEL maintainer="phithon <root@leavesongs.com>"

ADD https://github.com/vrana/adminer/releases/download/v4.6.2/adminer-4.6.2.php /var/www/html/index.php
RUN set -ex \
    && chown www-data:www-data -R /var/www/html \
    && docker-php-ext-install -j$(nproc) pdo_mysql mysqli
