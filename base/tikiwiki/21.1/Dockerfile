FROM php:7.2-apache

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && apt-get update \
    && apt-get install --no-install-recommends -y libfreetype6-dev libjpeg62-turbo-dev libpng-dev libicu-dev wait-for-it \
    && curl --retry 3 -#SL https://sourceforge.net/projects/tikiwiki/files/Tiki_21.x_UY_Scuti/21.1/tiki-21.1.tar.xz/download | tar xJ -C /var/www/html --strip-components=1 \
    && rm -rf /var/lib/apt/lists/*

RUN set -ex \
    && docker-php-ext-configure gd --with-freetype-dir=/usr --with-jpeg-dir=/usr --with-png-dir=/usr \
    && docker-php-ext-install -j$(nproc) gd intl calendar zip mysqli pdo_mysql \
    && cd /var/www/html \
    && chown www-data:www-data -R .
