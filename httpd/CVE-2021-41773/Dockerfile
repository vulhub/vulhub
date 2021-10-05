FROM vulhub/httpd:2.4.49

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && cat /usr/local/apache2/conf/httpd.conf \
        | tr '\n' '\r' \
        | perl -pe 's|<Directory />.*?</Directory>|<Directory />\n    AllowOverride none\n    Require all granted\n</Directory>|isg' \
        | tr '\r' '\n' \
        | tee /tmp/httpd.conf \
    && mv /tmp/httpd.conf /usr/local/apache2/conf/httpd.conf
