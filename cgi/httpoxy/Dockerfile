FROM vulhub/php:5.6.23-fpm

MAINTAINER phithon <root@leavesongs.com>

COPY www /var/www/html

RUN set -ex \
    && sed -i 's/deb.debian.org/mirrors.ustc.edu.cn/g' /etc/apt/sources.list \
    && apt-get update && apt-get install -y zip unzip \
    && wget -qO- https://getcomposer.org/installer | php -- --install-dir=/usr/local/bin --filename=composer \
    && cd /var/www/html \
    && composer config -g repo.packagist composer https://packagist.phpcomposer.com \
    && composer install \
    && apt-get purge -y --auto-remove wget zip unzip
