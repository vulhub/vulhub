FROM php:8.5-apache

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends curl unzip wait-for-it libsqlite3-dev libzip-dev \
    && rm -rf /var/lib/apt/lists/*

RUN set -ex \
    && docker-php-ext-install pdo_sqlite zip \
    && curl -#sSL https://getcomposer.org/download/2.9.3/composer.phar -o /usr/local/bin/composer \
    && chmod +x /usr/local/bin/composer

RUN set -ex \
    && cd /var/www/html \
    && composer create-project laravel/laravel:"v12.11.1" livewire-playground \
    && cd livewire-playground \
    && composer config audit.block-insecure false \
    && composer require livewire/livewire:3.6.3 \
    && php artisan make:livewire todo-list \
    && php artisan livewire:layout

WORKDIR /var/www/html/livewire-playground
COPY todo-list.blade.php /var/www/html/livewire-playground/resources/views/livewire/todo-list.blade.php
COPY TodoList.php /var/www/html/livewire-playground/app/Livewire/TodoList.php
COPY web.php /var/www/html/livewire-playground/routes/web.php

RUN set -ex \
    && a2enmod rewrite \
    && chown -R www-data:www-data /var/www/html/livewire-playground \
    && sed -i 's|DocumentRoot /var/www/html|DocumentRoot /var/www/html/livewire-playground/public|g' /etc/apache2/sites-enabled/000-default.conf
