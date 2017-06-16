FROM vulhub/php:5.6.23-fpm

MAINTAINER phithon <root@leavesongs.com>

COPY www /var/www/html

RUN set -ex \
	&& apt-get update && apt-get install -y zip unzip \
    && wget -qO- https://getcomposer.org/installer | php -- --install-dir=/usr/local/bin --filename=composer \
    && cd /var/www/html \
    && composer install \
    && apt-get purge -y --auto-remove wget zip unzip
