FROM php:7.4-apache

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && pecl install xdebug-3.1.6 \
    && docker-php-ext-enable xdebug

RUN set -ex \
    && { \
        echo "xdebug.mode = debug"; \
        echo "xdebug.discover_client_host = 1"; \
        echo "xdebug.log = /tmp/xdebug.log"; \
     } >> /usr/local/etc/php/conf.d/xdebug.ini \
    && echo "date.timezone = \"UTC\"" >> /usr/local/etc/php/conf.d/timezone.ini \
    && a2enmod rewrite

EXPOSE 80
