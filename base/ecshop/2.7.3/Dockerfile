FROM vulhub/php:5.3-apache

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends unzip \
    && curl -#SL https://vulhub.oss-cn-shanghai.aliyuncs.com/download/ecshop/ecshop-2.7.3.zip -o /tmp/ecshop-2.7.3.zip \
    && unzip /tmp/ecshop-2.7.3.zip \
    && cp -r /tmp/ECShop_2.7.3_UTF8_release1230/upload/* /var/www/html/ \
    && echo "date.timezone = Asia/Shanghai" > /usr/local/etc/php/conf.d/date.ini \
    && apt-get purge --auto-remove unzip \
    && rm -rf /var/lib/apt/lists/* /tmp/ECShop* /tmp/ecshop*