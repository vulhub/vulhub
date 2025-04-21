FROM php:7.4-apache

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends libfreetype-dev libjpeg62-turbo-dev libpng-dev libzip-dev wget ca-certificates \
    && docker-php-ext-configure gd --with-freetype --with-jpeg \
    && docker-php-ext-install -j$(nproc) gd zip

RUN set -ex \
    && cd /var/www/html \
    && wget -qO- https://github.com/star7th/showdoc/archive/refs/tags/v3.2.4.tar.gz | tar xz --strip-components=1 \
    && chown www-data:www-data -R .
