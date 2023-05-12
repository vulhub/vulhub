FROM php:5.5-apache

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && cd /tmp \
    && curl -#sSL https://github.com/vulhub/thinkphp-2.1/archive/master.tar.gz -o thinkphp-2.1.tar.gz \
    && tar -zxvf thinkphp-2.1.tar.gz \
    && cd thinkphp-2.1-master \
    && cp -r ThinkPHP Examples/Hello/* /var/www/html/ \
    && cd /var/www/html \
    && chown www-data:www-data -R . \
    && rm -rf /tmp/*

COPY index.php /var/www/html/index.php
