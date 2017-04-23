FROM php:5-fpm

MAINTAINER phithon <root@leavesongs.com>

RUN apt-get update && apt-get install -y \
        libfreetype6-dev \
        libjpeg62-turbo-dev \
        libmcrypt-dev \
        libpng12-dev \ 
        libbz2-dev \
        && docker-php-ext-install -j$(nproc) mysql mysqli iconv mcrypt zip bz2  \
        && docker-php-ext-configure gd --with-freetype-dir=/usr/include/ --with-jpeg-dir=/usr/include/ \
        && docker-php-ext-install -j$(nproc) gd

WORKDIR /var/www/html