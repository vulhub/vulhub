FROM php:8.3-apache

LABEL maintainer="phithon <root@leavesongs.com>"

RUN apt-get update \
    && apt-get install -y --no-install-recommends libjpeg62-turbo-dev libpng-dev libfreetype6-dev libonig-dev libicu-dev libzip-dev \
    && docker-php-ext-install bcmath gd mbstring pdo_mysql intl zip \
    && curl -#sSL -o /usr/local/bin/composer https://getcomposer.org/download/2.8.4/composer.phar \
    && chmod +x /usr/local/bin/composer \
    && rm -rf /var/lib/apt/lists/*

RUN set -ex \
    && curl -#sSL https://github.com/craftcms/craft/archive/refs/tags/5.1.0.tar.gz | tar xz --strip-components=1 -C /var/www/html \
    && cd /var/www/html \
    && sed -i 's|"craftcms/cms": "^5.0.0"|"craftcms/cms": "5.5.1.1"|g' composer.json \
    && sed -i 's|"craftcms/cms": "^5.0.0"|"craftcms/cms": "5.5.1.1"|g' composer.json.default \
    && composer install --no-scripts \
    && mv .env.example.dev .env \
    && rm composer.json \
    && mv composer.json.default composer.json \
    && composer dump-autoload -o \
    && CRAFT_ALLOW_SUPERUSER=1 php craft setup --interactive 0 \
    && chown -R www-data:www-data /var/www/html \
    && sed -i 's|DocumentRoot /var/www/html|DocumentRoot /var/www/html/web|g' /etc/apache2/sites-enabled/000-default.conf \
    && a2enmod rewrite

RUN set -ex \
    && { \
        echo "max_execution_time = 300"; \
        echo "ignore_user_abort = On"; \
    } > /usr/local/etc/php/conf.d/overrides.ini
