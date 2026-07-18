FROM wordpress:6.9.4-apache

LABEL maintainer="phithon <root@leavesongs.com>"

COPY auto-install.php /usr/local/bin/auto-install.php
COPY dynamic-siteurl.php /usr/src/wordpress/wp-content/mu-plugins/dynamic-siteurl.php
COPY docker-entrypoint-wrapper.sh /usr/local/bin/docker-entrypoint-wrapper.sh

RUN chmod +x /usr/local/bin/docker-entrypoint-wrapper.sh

ENTRYPOINT ["docker-entrypoint-wrapper.sh"]
CMD ["apache2-foreground"]
