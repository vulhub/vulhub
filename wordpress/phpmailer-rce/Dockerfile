FROM vulhub/httpd:bad-http

MAINTAINER phithon <root@leavesongs.com>

RUN set -ex; \
    mkdir -p /usr/src/wordpress; \
    wget -qO- https://github.com/WordPress/WordPress/archive/4.6.tar.gz | \
        tar zx -C /usr/src/wordpress --strip-components=1; \
	chown -R www-data:www-data /usr/src/wordpress; \
	rm -rf /usr/local/bin/docker-entrypoint.sh

ADD docker-entrypoint.sh /usr/local/bin/

RUN chmod +x /usr/local/bin/docker-entrypoint.sh \
	&& rm -rf /var/www/html/*