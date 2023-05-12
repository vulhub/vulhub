FROM debian:jessie

LABEL maintainer="phithon <root@leavesongs.com>"

COPY ./docker-entrypoint.sh /

RUN apt-get update -y && apt-get install -y --no-install-recommends \
    ca-certificates \
    curl \
    erlang-nox \
    erlang-reltool \
    haproxy \
    libicu52 \
    libmozjs185-1.0 \
    openssl \
    wget

RUN buildDeps=' \
    apt-transport-https \
    gcc \
    g++ \
    erlang-dev \
    libcurl4-openssl-dev \
    libicu-dev \
    libmozjs185-dev \
    make \
  ' \
    && apt-get update \
    && apt-get install -y --no-install-recommends $buildDeps \
    && rm -rf /var/lib/apt/lists/* \
    && cd /usr/src && mkdir couchdb \
    && wget -qO- https://archive.apache.org/dist/couchdb/source/2.1.0/apache-couchdb-2.1.0.tar.gz | tar zx -C couchdb --strip-components=1 \
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

# Add configuration
COPY vm.args /opt/couchdb/etc/

WORKDIR /opt/couchdb
EXPOSE 5984 4369 9100

ENTRYPOINT ["/docker-entrypoint.sh"]
CMD ["/opt/couchdb/bin/couchdb"]