FROM ubuntu:trusty

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

ADD docker-entrypoint.sh /usr/local/bin/docker-entrypoint.sh

RUN chmod +x /usr/local/bin/docker-entrypoint.sh \
	&& rm -rf /var/www/html/*

WORKDIR /var/www/html

ENTRYPOINT ["/usr/local/bin/docker-entrypoint.sh"]
CMD ["apache2ctl", "-DFOREGROUND"]