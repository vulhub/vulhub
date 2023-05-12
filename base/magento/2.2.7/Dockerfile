FROM php:7.1-apache

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && apt-get update && apt-get install -y \
        libfreetype6-dev \
        libjpeg62-turbo-dev \
        libpng-dev \
        libmcrypt-dev \
        libicu-dev \
        libxml2-dev \
        libxslt1-dev \
    && docker-php-ext-install -j$(nproc) iconv mcrypt bcmath intl zip pdo_mysql soap xsl \
    && docker-php-ext-configure gd --with-freetype-dir=/usr/include/ --with-jpeg-dir=/usr/include/ \
    && docker-php-ext-install -j$(nproc) gd \
    && rm -rf /usr/src/* /var/lib/apt/lists/*

RUN set -ex \
    && cd /var/www/html \
    && curl -sSL https://github.com/magento/magento2/archive/2.2.7.tar.gz | tar xvz --strip-components=1 \
    && curl -sSL https://getcomposer.org/installer | php -- --install-dir=/usr/local/bin --filename composer \
    && composer install \
    && chown www-data:www-data -R . \
    && a2enmod rewrite
