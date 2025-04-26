FROM debian:bullseye-slim

LABEL maintainer="phithon <root@leavesongs.com>"

RUN apt-get update -y && apt-get install -y --no-install-recommends \
    erlang-nox \
    erlang-reltool \
    libicu-dev \
    libcurl4-openssl-dev \
    libnspr4-dev \
    libffi-dev \
    libmozjs-78-dev

RUN buildDeps=' \
    gcc \
    g++ \
    erlang-dev \
    make \
    wget \
    ca-certificates \
    apt-transport-https \
  ' \
    && apt-get update \
    && apt-get install -y --no-install-recommends $buildDeps \
    && rm -rf /var/lib/apt/lists/* \
    && ln -s /usr/include/mozjs-78 /usr/local/include/mozjs-78 \
    && cd /usr/src && mkdir couchdb \
    && wget -qO- https://archive.apache.org/dist/couchdb/source/3.2.1/apache-couchdb-3.2.1.tar.gz | tar zx -C couchdb --strip-components=1 \
    && cd couchdb \
    # Build the release and install into /opt
    && ./configure --disable-docs --spidermonkey-version=78 \
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
    } | tee /opt/couchdb/etc/local.d/local.ini

# Add configuration
COPY ./docker-entrypoint.sh /

RUN chmod +x /docker-entrypoint.sh

WORKDIR /opt/couchdb
EXPOSE 5984 4369 9100

ENTRYPOINT ["/docker-entrypoint.sh"]
CMD ["/opt/couchdb/bin/couchdb"]
