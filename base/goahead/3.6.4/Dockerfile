FROM debian:jessie

MAINTAINER phithon <root@leavesongs.com>

COPY index.c /tmp/

RUN set -ex \
    && apt-get update \
    && apt-get install wget make gcc -y \
    && wget -qO- https://github.com/embedthis/goahead/archive/v3.6.4.tar.gz | tar zx --strip-components 1 -C /usr/src/ \
    && cd /usr/src \
    && make && make install \
    && cp src/self.key src/self.crt /etc/goahead/ \
    && cd / \
    && mkdir -p /var/www/goahead/cgi-bin/ \
    && gcc /tmp/index.c -o /var/www/goahead/cgi-bin/index \
    && apt-get purge -y --auto-remove wget make gcc \
    && rm -rf /usr/src/ /var/lib/apt/lists/* /tmp/index.c

CMD ["goahead", "-v", "--home", "/etc/goahead", "/var/www/goahead"]