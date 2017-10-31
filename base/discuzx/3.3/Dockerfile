FROM vulhub/php:5.5-apache

LABEL maintainer=<mengbulang@Live.com>

RUN apt-get update && apt-get install git php5-mysql -y \
    && docker-php-ext-install mysql mysqli \
    && apt-get clean \
    && cd /tmp/ \
    && git clone https://gitee.com/ComsenzDiscuz/DiscuzX.git \
    && cd DiscuzX \
    && git checkout 2efb765de0000ddf307616801afaa58f1d94d77f \
    # 2efb765de0000ddf307616801afaa58f1d94d77f is discuzx(Release 20170401)
    && cp -r /tmp/DiscuzX/upload/* /var/www/html \
    && cd /var/www/html \
    && chmod a+w -R ./* \
    && rm -rf /var/lib/apt/lists/* \
    && rm -rf /tmp/DiscuzX

EXPOSE 80