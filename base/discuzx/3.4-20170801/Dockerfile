FROM vulhub/php:5.5-apache

LABEL maintainer=<mengbulang@Live.com>

RUN set -ex \
    && apt-get update \
    && apt-get install git -y --no-install-recommends \
    && docker-php-ext-install mysql mysqli \
    && apt-get clean \
    && cd /tmp/ \
    && git clone https://gitee.com/ComsenzDiscuz/DiscuzX.git \
    && cd DiscuzX \
    && git checkout d7ec4030d5bcc2be9485a2543bab85b9757e3c14 \
    # d7ec4030d5bcc2be9485a2543bab85b9757e3c14 is discuzx(Release 20170801)
    && cp -r /tmp/DiscuzX/upload/* /var/www/html \
    && cd /var/www/html \
    && chmod a+w -R ./* \
    && rm -rf /var/lib/apt/lists/* \
    && rm -rf /tmp/DiscuzX

EXPOSE 80