FROM debian:buster

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && apt-get update \
    && apt-get install --no-install-recommends -y ca-certificates wget rrdtool \
    && apt-get install --no-install-recommends -y build-essential git bison flex libxml2-dev libpcap-dev libtool libtool-bin \
        librrd-dev autoconf pkg-config automake autogen redis-server libsqlite3-dev libhiredis-dev libmaxminddb-dev \
        libcurl4-openssl-dev libpango1.0-dev libcairo2-dev libnetfilter-queue-dev zlib1g-dev libssl-dev libcap-dev \
        libnetfilter-conntrack-dev libreadline-dev libjson-c-dev libldap2-dev rename libsnmp-dev libpng-dev libzmq5-dev default-libmysqlclient-dev \
    && cd /usr/src \
    && mkdir ntopng nDPI \
    && cd nDPI \
    && wget -O- https://github.com/ntop/nDPI/archive/refs/tags/3.4.tar.gz | tar xz --strip-components=1 \
    && ./autogen.sh && ./configure && make \
    && cd ../ntopng \ 
    && wget -O- https://github.com/ntop/ntopng/archive/2154003f683e6483d0f5bd23780581ba5668ef49.tar.gz | tar xz --strip-components=1 \
    && ./autogen.sh && ./configure && make \
    && make install \
    && cd / \
    && rm -rf /usr/src/* \
    && apt-get purge -y --auto-remove build-essential git bison flex autoconf pkg-config automake autogen redis-server libtool libtool-bin

ENV REDIS_SERVER="127.0.0.1:6379"
CMD ["bash", "-c", "ntopng --http-port=0.0.0.0:3000 --redis=${REDIS_SERVER}"]
