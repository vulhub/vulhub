FROM debian:jessie

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

RUN wget http://nginx.org/download/nginx-${NGINX_VERSION}.tar.gz -O /tmp/nginx-${NGINX_VERSION}.tar.gz \
    && tar -zxf /tmp/nginx-${NGINX_VERSION}.tar.gz -C /tmp/ \
    && wget https://ftp.pcre.org/pub/pcre/pcre-8.40.tar.gz -O /tmp/pcre-8.40.tar.gz \
    && tar -zxf /tmp/pcre-8.40.tar.gz -C /tmp/ \
    && wget http://zlib.net/zlib-1.2.11.tar.gz -O /tmp/zlib-1.2.11.tar.gz \
    && tar -zxf /tmp/zlib-1.2.11.tar.gz -C /tmp/ \
    && wget https://github.com/openssl/openssl/archive/OpenSSL_1_0_1c.tar.gz -O /tmp/OpenSSL_1_0_1c.tar.gz \
    && tar -zxf /tmp/OpenSSL_1_0_1c.tar.gz -C /tmp/ \
    && cd /tmp/nginx-${NGINX_VERSION} \
    && ./configure --with-http_ssl_module --with-pcre=../pcre-8.40 --with-zlib=../zlib-1.2.11 --with-openssl=../openssl-OpenSSL_1_0_1c \
    && make \
    && make install \
    && make clean \
    && cd / \
    && rm -rf /tmp/nginx-${NGINX_VERSION} /tmp/pcre-8.40 /tmp/zlib-1.2.11 /tmp/openssl-OpenSSL_1_0_1c

RUN apt-get purge -y --auto-remove -o APT::AutoRemove::RecommendsImportant=false $BUILD_TOOLS \
    && rm -rf /tmp/OpenSSL_1_0_1c.tar.gz /tmp/nginx-1.11.13.tar.gz /tmp/pcre-8.40.tar.gz /tmp/zlib-1.2.11.tar.gz

RUN touch /var/log/access.log \
    && touch /var/log/error.log \
    && ln -sf /dev/stdout /var/log/access.log \
	&& ln -sf /dev/stderr /var/log/error.log \
    && ln -sf /usr/local/nginx/sbin/nginx /usr/sbin/nginx

EXPOSE 80 443

CMD ["nginx", "-c", "/etc/nginx/nginx.conf", "-g", "daemon off;"]