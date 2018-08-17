FROM php:7.1-apache

MAINTAINER phithon <root@leavesongs.com>

RUN curl -#SL https://www.imagemagick.org/download/releases/ImageMagick-6.9.2-10.tar.xz \
     | tar xJ -C /tmp/ \
    && cd /tmp/ImageMagick-6.9.2-10 \
    && ./configure && make \
    && make install \
    && ldconfig /usr/local/lib

RUN pecl install imagick \
    && docker-php-ext-enable imagick

RUN rm -rf /tmp/ImageMagick*