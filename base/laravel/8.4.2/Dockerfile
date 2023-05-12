FROM php:7.4-apache

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends unzip \
    && curl -#L -o /usr/local/bin/composer https://github.com/composer/composer/releases/download/1.10.19/composer.phar \
    && chmod +x /usr/local/bin/composer

RUN set -ex \
    && cd /var/www \
    && rm -rf html \
    && composer create-project laravel/laravel . "v8.4.2" \
    && sed -i -E 's|"facade/ignition": ".+?"|"facade/ignition": "2.5.1"|g' composer.json \
    && composer update \
    && mv public html

RUN set -ex \
    && chown www-data:www-data -R /var/www \
    && a2enmod rewrite
