FROM debian:buster

LABEL maintainer="phithon <root@leavesongs.com>"

ADD https://github.com/openssl/openssl/archive/refs/tags/OpenSSL_1_1_1m.tar.gz /OpenSSL_1_1_1m.tar.gz

RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends make gcc perl libc6-dev \
    && cd /usr/src \
    && tar xzvf /OpenSSL_1_1_1m.tar.gz -C /usr/src --strip-components=1 \
    && ./config \
    && make \
    && make install \
    && apt-get purge -y --auto-remove gcc make perl \
    && rm -rf /usr/src/* /var/lib/apt/lists/*
