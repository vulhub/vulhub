FROM php:7.2-apache

LABEL maintainer="phithon <root@leavesongs.com>"

ARG MIRROR_URL=https://github.com/vulhub/mirrors/raw/master/debian/stretch/aliyun.list

RUN set -ex \
        && curl -#sL $MIRROR_URL -o /etc/apt/sources.list \
        && apt-get update \
        && apt-get install -y --no-install-recommends unzip \
        && rm -rf /var/lib/apt/lists/*

ENV APACHE_DOCUMENT_ROOT /var/www/public

RUN set -ex \
    && cd /var/www \
    && rm -rf * \
    && curl -#SL https://github.com/top-think/think/archive/v5.0.20.tar.gz | tar zx --strip-components=1 \
    && sed -ri -e 's!/var/www/html!${APACHE_DOCUMENT_ROOT}!g' /etc/apache2/sites-available/*.conf \
    && sed -ri -e 's!/var/www/!${APACHE_DOCUMENT_ROOT}!g' /etc/apache2/apache2.conf /etc/apache2/conf-available/*.conf

COPY composer.json /var/www/

RUN set -ex \
    && cd /var/www \
    && curl -#sSL https://getcomposer.org/download/2.3.10/composer.phar -o composer.phar \
    && php composer.phar install \
    && chown www-data:www-data -R .

WORKDIR /var/www/public