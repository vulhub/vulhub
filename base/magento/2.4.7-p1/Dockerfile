FROM php:8.1-apache

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
        libzip-dev \
        wait-for-it \
    && docker-php-ext-install -j$(nproc) iconv bcmath intl zip pdo_mysql soap xsl sockets \
    && docker-php-ext-configure gd --with-freetype --with-jpeg \
    && docker-php-ext-install -j$(nproc) gd \
    && rm -rf /usr/src/* /var/lib/apt/lists/*

RUN set -ex \
    && cd /var/www/html \
    && curl -sSL https://github.com/magento/magento2/archive/2.4.7-p1.tar.gz | tar xvz --strip-components=1 \
    && curl -#sSL -o /usr/local/bin/composer https://getcomposer.org/download/2.8.5/composer.phar \
    && chmod +x /usr/local/bin/composer \
    && composer install

COPY docker-entrypoint.sh /docker-entrypoint.sh
COPY default.ini /usr/local/etc/php/conf.d/default.ini
RUN set -ex \
    && cd /var/www/html \
    # && sed -i 's|DocumentRoot /var/www/html|DocumentRoot /var/www/html/pub|g' /etc/apache2/sites-enabled/000-default.conf \
    && chmod +x /docker-entrypoint.sh \
    && a2enmod rewrite

ENTRYPOINT ["/docker-entrypoint.sh"]
CMD ["apache2-foreground"]
