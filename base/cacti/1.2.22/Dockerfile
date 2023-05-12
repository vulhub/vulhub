FROM php:7.4-apache

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends rrdtool snmp wget ca-certificates libsnmp-dev default-mysql-client \
        wait-for-it libjpeg62-turbo-dev libpng-dev libfreetype6-dev libgmp-dev libldap2-dev

RUN set -ex \
    && docker-php-ext-configure gd --with-freetype --with-jpeg \
    && docker-php-ext-install pdo_mysql snmp gmp ldap sockets gd

RUN set -ex \
    && wget -qO- https://files.cacti.net/cacti/linux/cacti-1.2.22.tar.gz | tar zx -C /var/www/html --strip-components 1

COPY config.php /var/www/html/include/config.php
COPY cacti.ini /usr/local/etc/php/conf.d/cacti.ini
