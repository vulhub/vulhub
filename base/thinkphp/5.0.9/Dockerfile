FROM vulhub/php:7.1-apache

MAINTAINER phithon <root@leavesongs.com>

RUN set -ex \
        && apt-get update \
        && apt-get install -y --no-install-recommends git unzip \
        && docker-php-ext-install -j$(nproc) pdo_mysql

COPY composer.json /tmp/

RUN set -ex \
    && cd /var/www \
    && rm -rf * \
    && git clone https://github.com/top-think/think.git . \
    && git checkout v5.0.9 \
    && mv public html \
    && curl -sSL https://getcomposer.org/composer.phar -o composer.phar \
    && mv -f /tmp/composer.json ./ \
    && php composer.phar install \
    && chown www-data:www-data -R .

WORKDIR /var/www/html