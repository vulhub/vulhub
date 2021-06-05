FROM vulhub/php:5-fpm

LABEL maintainer="phithon <root@leavesongs.com>"

RUN rm -rf /var/www/html/* \
    && mkdir -p /var/www/html/uploadfiles \
    && chmod 777 /var/www/html/uploadfiles \
    && { \
        echo "#\!/bin/bash"; \
        echo "chmod 0777 /var/www/html/uploadfiles"; \
        echo "/usr/local/sbin/php-fpm"; \
        echo ; \
    } | tee /start.sh \
    && chmod +x /start.sh

COPY www.conf /usr/local/etc/php-fpm.d/www-2.conf

CMD ["/start.sh"]