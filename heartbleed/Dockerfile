FROM vulhub/debian:jessie

MAINTAINER phithon <root@leavesongs.com>

ENV NGINX_VERSION 1.11.13

ENV BUILD_TOOLS \
    gcc \
    libc-dev \
    make \
    g++ \
    wget

RUN apt-get update \
    && apt-get install -y $BUILD_TOOLS \
    && rm -rf /var/lib/apt/lists/*

RUN cd /tmp/ \
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
    && rm -rf /tmp/nginx-${NGINX_VERSION} /tmp/pcre-8.40 /tmp/zlib-1.2.11 /tmp/openssl-OpenSSL_1_0_1c

RUN apt-get purge -y --auto-remove -o APT::AutoRemove::RecommendsImportant=false $BUILD_TOOLS

RUN ln -sf /dev/stdout /var/log/access.log \
	&& ln -sf /dev/stderr /var/log/error.log \
    && ln -sf /usr/local/nginx/sbin/nginx /usr/sbin/nginx

RUN apt-get update \
    && apt-get install -y openssl \
    && mkdir -p /etc/ssl/nginx/ \
    && openssl req -x509 -nodes -days 365 -newkey rsa:2048 \
        -keyout /etc/ssl/nginx/local.key \
        -out /etc/ssl/nginx/local.crt \
        -subj "/C=US/ST=Denial/L=Springfield/O=Dis/CN=localhost" \
    && rm -rf /var/lib/apt/lists/* \
    && apt-get purge -y --auto-remove -o APT::AutoRemove::RecommendsImportant=false openssl

EXPOSE 80 443

CMD ["nginx", "-c", "/etc/nginx/nginx.conf", "-g", "daemon off;"]