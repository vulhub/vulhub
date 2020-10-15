FROM php:7.2-apache

LABEL maintainer="phith0n <root@leavesongs.com>"

RUN apt-get update; \
    apt-get install -y --no-install-recommends unzip; \
    curl -#sL https://getcomposer.org/download/1.10.15/composer.phar -o /usr/local/bin/composer; \
    chmod +x /usr/local/bin/composer; \
    cd /var/www/html; \
    composer require phpunit/phpunit:5.6.2; \
    apt-get purge --auto-remove -y unzip; \
    rm -rf /var/lib/apt/lists/*
