FROM php:7.1-apache

LABEL maintainer="phithon <root@leavesongs.com>"

COPY config.ini /var/www/html/

ENV GITLIST_LINK https://github.com/klaussilveira/gitlist/releases/download/0.6.0/gitlist-0.6.0.tar.gz

RUN apt-get update && apt-get install -y wget git \
	&& find /var/lib/apt/lists -type f -exec rm {} \; \
	&& curl -sL $GITLIST_LINK | tar --strip-components 1 -C /var/www/html -xzf - \
	&& mkdir /var/www/html/cache && chmod 777 /var/www/html/cache \
	&& a2enmod rewrite
