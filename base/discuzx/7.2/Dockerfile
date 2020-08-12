FROM php:5.6-apache

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && docker-php-ext-install mysql \
    && rm -rf /var/www/html/* \
    && cd /tmp \
    && curl -#SL https://github.com/vulhub/discuz7/archive/2009.11.26.tar.gz -o Discuz_7.2_FULL_SC_UTF8.tar.gz \
    && tar -zxvf Discuz_7.2_FULL_SC_UTF8.tar.gz \
    && cp -r Discuz_7.2_FULL_SC_UTF8/upload/* /var/www/html/ \
    && { \
        echo "request_order=GP"; \
        echo "date.timezone=Asia/Shanghai"; \
    } | tee /usr/local/etc/php/conf.d/core.ini \
    && chown www-data:www-data -R /var/www/html \
    && rm -rf /tmp/*