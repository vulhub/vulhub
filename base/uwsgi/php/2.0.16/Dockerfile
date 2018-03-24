FROM debian:stretch

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex && \
    apt-get update && \
    apt-get install --no-install-recommends -y libphp7.0-embed ca-certificates wget build-essential python-dev php7.0-dev libonig-dev libqdbm-dev libz-dev libxml2-dev && \
    wget -qO- https://github.com/unbit/uwsgi/archive/2.0.16.tar.gz | tar xz -C /usr/src --strip-components=1 && \
    # ln -s /usr/lib/x86_64-linux-gnu/libz3.so /usr/lib/x86_64-linux-gnu/libz.so && \
    cd /usr/src && \
    make && \
    python uwsgiconfig.py --plugin plugins/php && \
    python uwsgiconfig.py --plugin plugins/http && \
    cp uwsgi php_plugin.so http_plugin.so / && \
    mkdir -p /var/www && \
    apt-get purge --autoremove -y build-essential ca-certificates wget && \
    rm -rf /usr/src /var/lib/apt/lists/* && \
    echo '<?=phpinfo()?>' > /var/www/index.php

COPY uwsgi.ini /etc/uwsgi.ini

WORKDIR /

EXPOSE 8080

CMD ["/uwsgi", "/etc/uwsgi.ini"]