FROM vulhub/php:5.3-apache

LABEL maintainer="phithon <root@leavesongs.com>"

ARG DOWNLOAD_PATH=/usr/local/src

RUN set -ex \
    && curl -#SL https://vulhub.oss-cn-shanghai.aliyuncs.com/download/ecshop/ecshop-2.7.3.tar.gz \
        | tar zx -C ${DOWNLOAD_PATH} \
    && cp -r ${DOWNLOAD_PATH}/upload/* /var/www/html/ \
    && chown www-data:www-data -R /var/www/html \
    && echo "date.timezone = Asia/Shanghai" > /usr/local/etc/php/conf.d/date.ini \
    && rm -rf ${DOWNLOAD_PATH}/*