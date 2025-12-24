FROM php:7.1-apache

MAINTAINER phithon <root@leavesongs.com>

ENV APACHE_DOCUMENT_ROOT /var/www/html

RUN set -ex \
    && cd /var/www/html/ \
    && rm -rf * \
    && curl -#SL https://github.com/trippo/ResponsiveFilemanager/archive/v9.13.4.tar.gz | tar zx --strip-components=1  \
    && chown www-data:www-data -R .

WORKDIR /var/www/html