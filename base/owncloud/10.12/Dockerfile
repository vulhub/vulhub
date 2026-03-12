FROM owncloud/server:10.12

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && sed -i 's/,phpinfo//g' /etc/php/7.4/apache2/conf.d/99-owncloud-apache.ini
