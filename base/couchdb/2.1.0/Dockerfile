FROM debian:jessie

MAINTAINER phithon <root@leavesongs.com>

# Add configuration
COPY vm.args /opt/couchdb/etc/
COPY ./docker-entrypoint.sh /

RUN buildDeps=' \
    apt-transport-https \
    gcc \
    g++ \
    erlang-dev \
    libcurl4-openssl-dev \
    libicu-dev \
    libmozjs185-dev \
    make \
    git \
  ' \
    && apt-get update \
    && apt-get install -y --no-install-recommends $buildDeps \
    && apt-get install -y wget \
    && rm -rf /var/lib/apt/lists/* \
    && cd /usr/src && mkdir couchdb \
    && wget -qO- https://github.com/apache/couchdb/archive/2.1.0.tar.gz | tar zx -C couchdb --strip-components=1 \
    && cd couchdb \
    # Build the release and install into /opt
    && ./configure --disable-docs \
    && make release \
    && mv /usr/src/couchdb/rel/couchdb /opt/ \
    # Cleanup build detritus
    && apt-get purge -y --auto-remove $buildDeps \
    && rm -rf /var/lib/apt/lists/* /usr/src/couchdb* \
    && mkdir /opt/couchdb/data \
    && { \
        echo "[chttpd]"; \
        echo "bind_address = any"; \
        echo ; \
        echo "[httpd]"; \
        echo "bind_address = any"; \
        echo ; \
    } | tee /opt/couchdb/etc/local.d/local.ini \
    && chmod +x /docker-entrypoint.sh

WORKDIR /opt/couchdb
EXPOSE 5984 4369 9100

ENTRYPOINT ["/docker-entrypoint.sh"]
CMD ["/opt/couchdb/bin/couchdb"]