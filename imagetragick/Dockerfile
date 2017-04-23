FROM php:apache

MAINTAINER phithon <root@leavesongs.com>

RUN apt-get update && apt-get install -y wget \
    && wget -qO- https://www.imagemagick.org/download/releases/ImageMagick-6.9.2-10.tar.xz \
     | tar xJ -C /tmp/ \
    && cd /tmp/ImageMagick-6.9.2-10 \
    && ./configure && make \
    && make install \
    && ldconfig /usr/local/lib

RUN pecl install imagick \
    && docker-php-ext-enable imagick

RUN rm -rf /var/lib/apt/lists/* \
    && apt-get purge -y --auto-remove -o APT::AutoRemove::RecommendsImportant=false wget \
    && rm -rf /tmp/ImageMagick*

RUN sed -i -e "s/Options -Indexes/Options +Indexes/g" $APACHE_CONFDIR/conf-available/docker-php.conf