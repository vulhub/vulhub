FROM debian:stretch

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends apache2 libapache2-mod-php php-common php-zip php-mbstring php-curl php-gd php-soap php-xml php-intl php-bcmath php-mcrypt php-mysql wget unzip git ca-certificates \
    && wget -qO /usr/local/bin/composer https://github.com/composer/composer/releases/download/1.10.7/composer.phar \
    && chmod +x /usr/local/bin/composer

RUN set -ex \
    && cd /var/www/html \
    && wget -qO- https://github.com/magento/magento2/archive/2.2.7.tar.gz | tar xz --strip-components=1 \
    && composer install \
    && chown www-data:www-data -R . \
    && a2enmod rewrite

EXPOSE 80
CMD [ "apache2ctl", "-DFOREGROUND" ]
