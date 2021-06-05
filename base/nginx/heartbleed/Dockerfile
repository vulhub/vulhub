FROM debian:stretch

LABEL maintainer="phithon <root@leavesongs.com>"

ENV NGINX_VERSION 1.11.13

ENV BUILD_TOOLS \
    gcc \
    libc-dev \
    make \
    g++ \
    wget

RUN set -ex \
    && apt-get update \
    && apt-get install -y $BUILD_TOOLS \
    && rm -rf /var/lib/apt/lists/* \
    && cd /tmp/ \
    && wget -qO- http://nginx.org/download/nginx-${NGINX_VERSION}.tar.gz | tar zx -C /tmp/ \
    && wget -qO- https://ftp.pcre.org/pub/pcre/pcre-8.40.tar.gz | tar zx -C /tmp/ \
    && wget -qO- http://zlib.net/zlib-1.2.11.tar.gz | tar zx -C /tmp/ \
    && wget -qO- https://github.com/openssl/openssl/archive/OpenSSL_1_0_1c.tar.gz | tar zx -C /tmp/ \
    && cd /tmp/nginx-${NGINX_VERSION} \
    && ./configure \
        --with-http_ssl_module \
        --with-pcre=../pcre-8.40 \
        --with-zlib=../zlib-1.2.11 \
        --with-openssl=../openssl-OpenSSL_1_0_1c \
    && make \
    && make install \
    && make clean \
    && cd / \
    && rm -rf /tmp/nginx-${NGINX_VERSION} /tmp/pcre-8.40 /tmp/zlib-1.2.11 /tmp/openssl-OpenSSL_1_0_1c \
    && apt-get purge -y --auto-remove -o APT::AutoRemove::RecommendsImportant=false $BUILD_TOOLS

RUN set -ex \
    && mkdir -p /etc/ssl/nginx/ /etc/nginx/

COPY local.crt local.key /etc/ssl/nginx/
COPY nginx.conf /etc/nginx/
CMD ["/usr/local/nginx/sbin/nginx", "-c", "/etc/nginx/nginx.conf", "-g", "daemon off;"]
