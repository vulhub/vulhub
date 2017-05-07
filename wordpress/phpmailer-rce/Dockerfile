FROM vulhub/ubuntu:trusty

MAINTAINER phithon <root@leavesongs.com>

# install the PHP extensions we need
RUN set -ex; \
	\
	apt-get update; \
	apt-get install -y \
		apache2 \
		php5 \
		php5-gd \
		php5-curl \
		php5-mysqlnd \
        exim4 \
        wget \
		curl \
	; \
	rm -rf /var/lib/apt/lists/*

RUN a2enmod rewrite expires

RUN set -ex; \
    mkdir -p /usr/src/wordpress; \
    wget -qO- https://github.com/WordPress/WordPress/archive/4.6.tar.gz | \
        tar zx -C /usr/src/wordpress --strip-components=1; \
	chown -R www-data:www-data /usr/src/wordpress

COPY docker-entrypoint.sh /usr/local/bin/

RUN chmod +x /usr/local/bin/docker-entrypoint.sh \
	&& rm -rf /var/www/html/*

WORKDIR /var/www/html

ENTRYPOINT ["/usr/local/bin/docker-entrypoint.sh"]
CMD ["apache2ctl", "-DFOREGROUND"]