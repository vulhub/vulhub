FROM vulhub/bash:4.3.0

LABEL maintainer="phithon <root@leavesongs.com>"

ARG HTTPD_CONF=/etc/apache2

RUN apt-get update \
    && apt-get install --no-install-recommends -y apache2 \
    && { \
        echo; \
        echo "<Directory \"/var/www/html\">"; \
        echo '  AddHandler cgi-script .cgi'; \
        echo '  AllowOverride None'; \
        echo '  Options Indexes FollowSymLinks ExecCGI'; \
        echo '  Require all granted'; \
        echo '</Directory>'; \
        echo; \
    } >> ${HTTPD_CONF}/apache2.conf \
    && a2enmod cgi \
    && a2enmod cgid

CMD ["apache2ctl", "-DFOREGROUND"]
