FROM php:8.3-apache

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends unzip \
    && curl -sSLo /tmp/pdfjs-4.1.392-dist.zip https://github.com/mozilla/pdf.js/releases/download/v4.1.392/pdfjs-4.1.392-dist.zip \
    && unzip -d /var/www/html /tmp/pdfjs-4.1.392-dist.zip \
    && rm -rf /tmp/pdfjs-4.1.392-dist.zip /var/lib/apt/lists/

RUN set -ex \
    && mkdir /var/www/html/upload \
    && chown www-data:www-data -R /var/www/html
