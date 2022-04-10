FROM vulhub/openssl:1.0.1c

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends gcc make g++ wget ca-certificates \
    && cd /usr/src \
    && wget -qO- http://nginx.org/download/nginx-1.11.13.tar.gz | tar zx \
    && wget -qO- https://sourceforge.net/projects/pcre/files/pcre/8.45/pcre-8.45.tar.gz/download | tar zx \
    && wget -qO- https://sourceforge.net/projects/libpng/files/zlib/1.2.11/zlib-1.2.11.tar.gz/download | tar zx \
    \
    && cd nginx-1.11.13 \
    && sed -i 's/CFLAGS -Werror/CFLAGS/g' auto/cc/gcc \
    && ./configure --with-http_ssl_module --with-pcre=../pcre-8.45 --with-zlib=../zlib-1.2.11 --with-cc-opt="-I /usr/local/include" --with-ld-opt="-L/usr/local/lib -ldl -Wl,-rpath,/usr/local/lib" \
    && make \
    && make install \
    && apt-get purge -y --auto-remove gcc make g++ wget ca-certificates \
    && rm -rf /usr/src/* /var/lib/apt/lists/*

RUN set -ex \
    && mkdir -p /etc/ssl/nginx/ /etc/nginx/

COPY local.crt local.key /etc/ssl/nginx/
COPY nginx.conf /etc/nginx/
CMD ["/usr/local/nginx/sbin/nginx", "-c", "/etc/nginx/nginx.conf", "-g", "daemon off;"]
