FROM debian:buster

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && apt-get update \
    && apt-get install wget make gcc -y \
    && wget -qO- https://github.com/embedthis/goahead/archive/refs/tags/v5.1.4.tar.gz | tar zx --strip-components 1 -C /usr/src/ \
    && cd /usr/src \
    && make SHOW=1 ME_GOAHEAD_UPLOAD_DIR="'\"/tmp\"'" \
    && make install \
    && cp src/self.key src/self.crt /etc/goahead/ \
    && mkdir -p /var/www/goahead/cgi-bin/ \
    && apt-get purge -y --auto-remove wget make gcc \
    && cd /var/www/goahead \
    && rm -rf /usr/src/ /var/lib/apt/lists/* /tmp/index.c \
    && sed -e 's!^# route uri=/cgi-bin dir=cgi-bin handler=cgi$!route uri=/cgi-bin dir=/var/www/goahead handler=cgi!' -i /etc/goahead/route.txt

EXPOSE 80
CMD ["goahead", "-v", "--home", "/etc/goahead", "/var/www/goahead"]
