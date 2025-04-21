FROM php:7.1-apache

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && pecl install xdebug-2.5.5 \
    && docker-php-ext-enable xdebug

RUN set -ex \
    && { \
        echo "xdebug.remote_enable = 1"; \
        echo "xdebug.remote_connect_back = 1"; \
        echo "xdebug.remote_log = /tmp/xdebug.log"; \
     } >> /usr/local/etc/php/conf.d/xdebug.ini \
    && echo "date.timezone = \"UTC\"" >> /usr/local/etc/php/conf.d/timezone.ini \
    && a2enmod rewrite

EXPOSE 80
