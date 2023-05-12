FROM php:7.4-apache

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
        && apt-get update \
        && apt-get install -y --no-install-recommends unzip wait-for-it libjpeg62-turbo-dev libpng-dev libfreetype6-dev \
        && rm -rf /var/lib/apt/lists/*

RUN set -ex \
    && docker-php-ext-configure gd --with-freetype --with-jpeg \
    && docker-php-ext-install pdo_mysql gd pcntl \
    && yes '' | pecl install redis \
    && { \
        echo "extension=redis.so"; \
    } > /usr/local/etc/php/conf.d/redis.ini \
    && curl -#sSL https://getcomposer.org/download/2.4.4/composer.phar -o /usr/local/bin/composer \
    && chmod +x /usr/local/bin/composer

ENV APACHE_DOCUMENT_ROOT /var/www/public

RUN set -ex \
    && cd /var/www \
    && rm -rf * \
    && curl -#SL https://github.com/v2board/v2board/archive/refs/tags/1.6.1.tar.gz | tar zx --strip-components=1 \
    && sed -ri -e 's!/var/www/html!${APACHE_DOCUMENT_ROOT}!g' /etc/apache2/sites-available/*.conf \
    && sed -ri -e 's!/var/www/!${APACHE_DOCUMENT_ROOT}!g' /etc/apache2/apache2.conf /etc/apache2/conf-available/*.conf

RUN set -ex \
    && cd /var/www \
    && composer install \
    && a2enmod rewrite \
    && chown www-data:www-data -R .

COPY apache.htaccess /var/www/public/.htaccess
WORKDIR /var/www
