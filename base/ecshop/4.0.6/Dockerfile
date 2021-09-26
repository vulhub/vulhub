FROM php:5.6-apache

LABEL maintainer="phithon <root@leavesongs.com>"

ARG DOWNLOAD_PATH=/usr/local/src

RUN set -ex \
    && docker-php-ext-install mysqli \
    && curl -#SL http://vulhub.oss-cn-shanghai.aliyuncs.com/download/ecshop/ecshop-4.0.6.tar.gz \
        | tar zx -C ${DOWNLOAD_PATH} \
    && cp -r ${DOWNLOAD_PATH}/ecshop/* /var/www/html/ \
    && cp -r ${DOWNLOAD_PATH}/appserver /var/www/ \
    && chown www-data:www-data -R /var/www \
    && echo "date.timezone = Asia/Shanghai" > /usr/local/etc/php/conf.d/date.ini \
    && rm -rf ${DOWNLOAD_PATH}/*
