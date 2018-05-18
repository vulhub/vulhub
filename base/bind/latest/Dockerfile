FROM debian:stretch

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends bind9 dnsutils \
    && rm -rf /var/lib/apt/lists/*

COPY named.conf.options /etc/bind/named.conf.options

EXPOSE 53
EXPOSE 53/udp

CMD /usr/sbin/named -4 -c /etc/bind/named.conf -u bind -f