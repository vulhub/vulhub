FROM debian:jessie

LABEL maintainer="phithon <root@leavesongs.com>"

ENV NGINX_VERSION 1.4.2

ENV BUILD_TOOLS \
    gcc \
    libc-dev \
    make \
    g++ \
    wget

RUN set -ex \
    && apt-get update \
    && apt-get install -y $BUILD_TOOLS libssl-dev \
    && rm -rf /var/lib/apt/lists/* \
    && cd /tmp/ \
    && wget -qO- http://nginx.org/download/nginx-${NGINX_VERSION}.tar.gz | tar zx -C /tmp/ \
    && wget -qO- https://ftp.pcre.org/pub/pcre/pcre-8.40.tar.gz | tar zx -C /tmp/ \
    && wget -qO- http://zlib.net/zlib-1.2.11.tar.gz | tar zx -C /tmp/ \
    && cd /tmp/nginx-${NGINX_VERSION} \
    && ./configure \
        --with-http_ssl_module \
        --with-pcre=../pcre-8.40 \
        --with-zlib=../zlib-1.2.11 \
    && make \
    && make install \
    && make clean \
    && cd / \
    && rm -rf /tmp/nginx-${NGINX_VERSION} /tmp/pcre-8.40 /tmp/zlib-1.2.11 \
    && apt-get purge -y --auto-remove -o APT::AutoRemove::RecommendsImportant=false $BUILD_TOOLS

RUN ln -sf /dev/stdout /usr/local/nginx/logs/access.log \
	&& ln -sf /dev/stderr /usr/local/nginx/logs/error.log \
    && ln -sf /usr/local/nginx/sbin/nginx /usr/sbin/nginx

EXPOSE 80 443

CMD ["nginx", "-g", "daemon off;"]