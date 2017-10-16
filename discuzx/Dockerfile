FROM php:5-apache

LABEL maintainer=<mengbulang@Live.com>

RUN apt-get update && apt-get install git php5-mysql -y \
    && docker-php-ext-install mysql mysqli

RUN cd /tmp/ \
    && git clone https://gitee.com/ComsenzDiscuz/DiscuzX.git \
    && cd DiscuzX \
    && git checkout 1a912ddb4a62364d1736fa4578b42ecc62c5d0be

RUN cp -r /tmp/DiscuzX/upload/* /var/www/html \
    && cd /var/www/html \
    && touch target.txt \
    && chmod a+w -R config data uc_server/data uc_client/data \
    && rm -rf /var/lib/apt/lists/*

EXPOSE 9999
