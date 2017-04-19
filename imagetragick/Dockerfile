FROM php:apache

RUN apt-get update && apt-get install -y wget \
    && wget -qO- https://www.imagemagick.org/download/releases/ImageMagick-6.9.2-10.tar.xz \
     | tar xJ -C /tmp/ \
    && cd /tmp/ \
    && ./configure && make \
    && make install \
    && ldconfig /usr/local/lib

RUN pecl install imagick \
    && docker-php-ext-enable imagick

RUN rm -rf /var/lib/apt/lists/* \
    && apt-get purge -y --auto-remove -o APT::AutoRemove::RecommendsImportant=false wget \
    && rm -rf /tmp/ImageMagick*

RUN { \
        echo; \
        echo "<Directory \"/var/www/html\">"; \
        echo '  Options Indexes'; \
        echo '  Require all granted'; \
        echo '</Directory>'; \
        echo; \
    } >> ${HTTPD_PREFIX}/conf/httpd.conf