FROM vulhub/php:5.4.1

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends \
		apache2 \
	&& rm -rf /var/lib/apt/lists/* \
    && \
    { \
        echo; \
        echo '<Directory "/usr/local/bin">'; \
        echo '  Require all granted'; \
        echo '  Options ExecCGI'; \
        echo '</Directory>'; \
        echo; \
        echo 'ScriptAlias /local-bin /usr/local/bin'; \
        echo; \
        echo "<Directory \"/var/www/html\">"; \
        echo '  AddHandler application/x-httpd-php5 php'; \
        echo '  Action application/x-httpd-php5 /local-bin/php-cgi'; \
        echo '  Options -Indexes'; \
        echo '  Require all granted'; \
        echo '  DirectoryIndex index.html index.php'; \
        echo '</Directory>'; \
        echo; \
    } >> /etc/apache2/conf-available/php.conf \
    && a2enmod cgid \
    && a2enmod actions \
    && a2disconf serve-cgi-bin \
    && a2enconf php

COPY apache2-foreground /usr/local/bin/

WORKDIR /var/www/html

EXPOSE 80

CMD ["apache2-foreground"]