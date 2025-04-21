FROM vulhub/httpd:2.4.43

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && sed -i "s|#LoadModule proxy_module modules/mod_proxy.so|LoadModule proxy_module modules/mod_proxy.so|g" /usr/local/apache2/conf/httpd.conf \
    && sed -i "s|#LoadModule proxy_http_module modules/mod_proxy_http.so|LoadModule proxy_http_module modules/mod_proxy_http.so|g" /usr/local/apache2/conf/httpd.conf \
    && sed -i "s|#LoadModule proxy_ajp_module modules/mod_proxy_ajp.so|LoadModule proxy_ajp_module modules/mod_proxy_ajp.so|g" /usr/local/apache2/conf/httpd.conf \
    && sed -i "s|#Include conf/extra/httpd-vhosts.conf|Include conf/extra/httpd-vhosts.conf|g" /usr/local/apache2/conf/httpd.conf \
    && { \
        echo '<VirtualHost *>'; \
        echo '  ServerAdmin webmaster@localhost'; \
        echo '  ServerName localhost'; \
        echo '  DocumentRoot /usr/local/apache2/htdocs'; \
        echo ; \
        echo '  ProxyPass / "ajp://tomcat:8009/" disablereuse=On'; \
        echo '</VirtualHost>'; \
    } > /usr/local/apache2/conf/extra/httpd-vhosts.conf
