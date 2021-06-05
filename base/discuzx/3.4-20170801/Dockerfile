FROM php:5.5-apache

LABEL maintainer=<mengbulang@Live.com>

RUN set -ex \
    && apt-get update \
    && apt-get install git -y --no-install-recommends \
    && docker-php-ext-install mysql mysqli \
    && apt-get clean \
    && cd /tmp/ \
    && git clone https://gitee.com/Discuz/DiscuzX.git \
    && cd DiscuzX \
    && git checkout d7ec4030d5bcc2be9485a2543bab85b9757e3c14 \
    # d7ec4030d5bcc2be9485a2543bab85b9757e3c14 is discuzx(Release 20170801)
    && cp -r /tmp/DiscuzX/upload/* /var/www/html \
    && chown www-data:www-data -R /var/www/html/ \
    && rm -rf /var/lib/apt/lists/* \
    && rm -rf /tmp/DiscuzX

EXPOSE 80
