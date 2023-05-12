FROM php:7.4-apache

LABEL MAINTAINER="phithon <root@leavesongs.com>"

RUN set -ex \
        && apt-get update \
        && apt-get install -y --no-install-recommends unzip \
        && rm -rf /var/lib/apt/lists/*

ENV APACHE_DOCUMENT_ROOT /var/www/public

RUN set -ex \
    && cd /var/www \
    && rm -rf * \
    && curl -#SL https://github.com/top-think/think/archive/v6.0.12.tar.gz | tar zx --strip-components=1 \
    && sed -ri -e 's!/var/www/html!${APACHE_DOCUMENT_ROOT}!g' /etc/apache2/sites-available/*.conf \
    && sed -ri -e 's!/var/www/!${APACHE_DOCUMENT_ROOT}!g' /etc/apache2/apache2.conf /etc/apache2/conf-available/*.conf

COPY composer.json /var/www/

RUN set -ex \
    && cd /var/www \
    && curl -#sSL https://getcomposer.org/download/2.4.4/composer.phar -o composer.phar \
    && php composer.phar install \
    && sed -i 's|// \\think\\middleware\\LoadLangPack.*|   \\think\\middleware\\LoadLangPack::class,|i' app/middleware.php \
    && chown www-data:www-data -R .

WORKDIR /var/www/public
