FROM vulhub/httpd:2.4.50

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && sed -i "s|#LoadModule cgid_module modules/mod_cgid.so|LoadModule cgid_module modules/mod_cgid.so|g" /usr/local/apache2/conf/httpd.conf \
    && sed -i "s|#LoadModule cgi_module modules/mod_cgi.so|LoadModule cgi_module modules/mod_cgi.so|g" /usr/local/apache2/conf/httpd.conf \
    && sed -i "s|#Include conf/extra/httpd-autoindex.conf|Include conf/extra/httpd-autoindex.conf|g" /usr/local/apache2/conf/httpd.conf \
    && cat /usr/local/apache2/conf/httpd.conf \
        | tr '\n' '\r' \
        | perl -pe 's|<Directory />.*?</Directory>|<Directory />\n    AllowOverride none\n    Require all granted\n</Directory>|isg' \
        | tr '\r' '\n' \
        | tee /tmp/httpd.conf \
    && mv /tmp/httpd.conf /usr/local/apache2/conf/httpd.conf
