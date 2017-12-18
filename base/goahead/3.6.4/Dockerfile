FROM debian:jessie

MAINTAINER phithon <root@leavesongs.com>

RUN set -ex \
    && apt-get update \
    && apt-get install wget make gcc -y \
    && wget -qO- https://github.com/embedthis/goahead/archive/v3.6.4.tar.gz | tar zx --strip-components 1 -C /usr/src/ \
    && cd /usr/src \
    && make && make install \
    && cp src/self.key src/self.crt /etc/goahead/ \
    && cd / \
    && apt-get purge -y --auto-remove wget make gcc \
    && rm -rf /usr/src/ /var/lib/apt/lists/*

CMD ["goahead", "-v", "--home", "/etc/goahead", "/var/www/goahead"]