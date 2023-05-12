FROM vulhub/php:5.6-apache

LABEL maintainer="phithon <root@leavesongs.com>"

COPY www/* /var/www/html/

RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends git \
    && cd /var/www/html/ \
    && curl -sSL https://getcomposer.org/installer | php \
    && php composer.phar install \
    && rm -rf /var/lib/apt/lists/*