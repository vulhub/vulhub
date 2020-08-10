FROM buildpack-deps:stretch-curl

RUN set -ex \
    && apt-get update \
    && apt-get install --no-install-recommends -y make gcc libc6-dev \
    && mkdir -p /usr/src/appweb \
    && cd /usr/src/appweb \
    && wget -qO- https://github.com/vulhub/appweb7/archive/v7.0.1.tar.gz | tar zx --strip-components=1 \
    && make && make install \
    && cd / \
    && apt-get purge -y --auto-remove gcc make \
    && rm -rf /usr/src/* /var/lib/apt/lists/*

RUN { \
        echo 'Documents "/var/www/appweb"'; \
        echo "Listen 8080"; \
        echo "<if SSL_MODULE>"; \
        echo "  ListenSecure 8443"; \
        echo "</if>"; \
    } | tee /etc/appweb/install.conf \
    && sed -i 's|ErrorLog "error.log"|ErrorLog "/dev/stdout"|g' /etc/appweb/appweb.conf

WORKDIR /etc/appweb

CMD ["appweb"]