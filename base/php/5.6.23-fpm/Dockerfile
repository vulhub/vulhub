FROM php:5.6.23-fpm

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
	&& apt-get update \
	&& apt-get install -y --no-install-recommends unzip git \
	&& curl -sSL -o /usr/local/bin/composer https://github.com/composer/composer/releases/download/1.6.4/composer.phar \
	&& chmod +x /usr/local/bin/composer