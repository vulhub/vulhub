FROM php:5.4-apache
LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && docker-php-ext-install iconv mbstring mysqli \
    && rm -r /var/lib/apt/lists/*

RUN set -ex \
    && curl -#SL https://github.com/phpmyadmin/phpmyadmin/archive/RELEASE_2_8_0_4.tar.gz | tar zx -C /var/www/html --strip-components=1