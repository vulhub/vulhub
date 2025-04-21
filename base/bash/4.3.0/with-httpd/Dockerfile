FROM vulhub/bash:4.3.0

LABEL maintainer="phithon <root@leavesongs.com>"

RUN apt-get update \
    && apt-get install --no-install-recommends -y apache2 \
    && a2enmod cgi \
    && a2enmod cgid

RUN set -ex \
    && { \
        echo; \
        echo "<Directory \"/var/www/html\">"; \
        echo '  AddHandler cgi-script .cgi'; \
        echo '  AllowOverride None'; \
        echo '  Options Indexes FollowSymLinks ExecCGI'; \
        echo '  Require all granted'; \
        echo '</Directory>'; \
        echo; \
    } >> /etc/apache2/apache2.conf

CMD ["apache2ctl", "-DFOREGROUND"]
