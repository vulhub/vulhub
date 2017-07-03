FROM vulhub/php:7.1-apache

MAINTAINER phithon <root@leavesongs.com>

RUN set -ex \
        && apt-get update && apt-get install -y \
        libfreetype6-dev \
        libjpeg62-turbo-dev \
        libmcrypt-dev \
        libpng12-dev \ 
        libbz2-dev \
        && apt-get install -y --no-install-recommends git \
        && docker-php-ext-install -j$(nproc) pdo_mysql iconv mcrypt zip bz2  \
        && docker-php-ext-configure gd --with-freetype-dir=/usr/include/ --with-jpeg-dir=/usr/include/ \
        && docker-php-ext-install -j$(nproc) gd

RUN set -ex \
    && cd /var/www \
    && rm -rf * \
    && git clone https://github.com/top-think/think.git . \
    && git checkout v5.0.9 \
    && mv public html \
    && curl https://getcomposer.org/composer.phar -o composer.phar \
    && php composer.phar install \
    && chown www-data:www-data -R .

WORKDIR /var/www/html