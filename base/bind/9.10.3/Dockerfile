FROM debian:stretch

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && echo "deb http://archive.debian.org/debian stretch main" > /etc/apt/sources.list \
    && apt-get -o Acquire::Check-Valid-Until=false update \
    && apt-get install -y --no-install-recommends bind9=1:9.10.3.dfsg.P4-12.3+deb9u6 dnsutils \
    && rm -rf /var/lib/apt/lists/*

COPY named.conf.options /etc/bind/named.conf.options

EXPOSE 53
EXPOSE 53/udp

CMD /usr/sbin/named -4 -c /etc/bind/named.conf -u bind -f