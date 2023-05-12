FROM debian:buster

LABEL maintainer="phithon <root@leavesongs.com>"

ADD https://github.com/openssl/openssl/archive/OpenSSL_1_0_1c.tar.gz /OpenSSL_1_0_1c.tar.gz

RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends make gcc perl libc6-dev \
    && cd /usr/src \
    && tar xzvf /OpenSSL_1_0_1c.tar.gz -C /usr/src --strip-components=1 \
    && ./config --prefix=/usr/local --openssldir=/usr/local/openssl shared \
    && make \
    && make install_sw \
    && apt-get purge -y --auto-remove gcc make perl \
    && rm -rf /usr/src/* /var/lib/apt/lists/*
