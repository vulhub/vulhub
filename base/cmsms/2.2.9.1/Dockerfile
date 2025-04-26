FROM php:7.2-apache

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends unzip libfreetype6-dev libjpeg62-turbo-dev libpng-dev libzip-dev \
    && docker-php-ext-configure gd --with-freetype-dir=/usr/include/ --with-jpeg-dir=/usr/include/ \
    && docker-php-ext-install -j$(nproc) gd mysqli zip \
    && echo "output_buffering=4096\ndisplay_errors=Off\nerror_reporting=0" >> /usr/local/etc/php/conf.d/cmsms.ini \
    && curl -L -o /tmp/cmsms-2.2.9.1-install.zip http://s3.amazonaws.com/cmsms/downloads/14316/cmsms-2.2.9.1-install.zip \
    && unzip -d /var/www/html /tmp/cmsms-2.2.9.1-install.zip \
    && mv /var/www/html/cmsms-2.2.9.1-install.php /var/www/html/install.php \
    && chown www-data:www-data -R /var/www/html \
    && a2enmod rewrite \
    && rm -rf /tmp/cmsms-2.2.9.1-install.zip

EXPOSE 80
