FROM vulhub/imagemagick:6.9.2-10

LABEL maintainer="phithon <root@leavesongs.com>"

RUN sed -i -e "s/Options -Indexes/Options +Indexes/g" $APACHE_CONFDIR/conf-available/docker-php.conf