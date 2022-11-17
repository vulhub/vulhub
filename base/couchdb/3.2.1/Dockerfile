FROM debian:bullseye-slim AS builder

RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends lsb-release wget ca-certificates make devscripts build-essential \
        debhelper libffi-dev libnspr4-dev autotools-dev zip pkg-kde-tools python pkg-config  \
    && cd /usr/src \
    && wget -qO- https://github.com/apache/couchdb-pkg/archive/refs/tags/3.2.1-1.tar.gz | tar zx --strip-components=1 \
    && make couch-js-debs PLATFORM=$(lsb_release -cs)

FROM debian:bullseye-slim

LABEL maintainer="phithon <root@leavesongs.com>"

RUN apt-get update -y && apt-get install -y --no-install-recommends \
    ca-certificates \
    apt-transport-https \
    curl \
    erlang-nox \
    erlang-reltool \
    haproxy \
    libicu-dev \
    openssl \
    wget \
    libcurl4-openssl-dev \
    libnspr4-dev \
    libffi-dev

COPY --from=builder /usr/src/js/*.deb /tmp/

RUN buildDeps=' \
    gcc \
    g++ \
    erlang-dev \
    make \
  ' \
    && apt-get update \
    && apt-get install -y --no-install-recommends $buildDeps \
    && rm -rf /var/lib/apt/lists/* \
    && dpkg -i /tmp/couch-libmozjs185-1.0_1.8.5*.deb \
    && dpkg -i /tmp/couch-libmozjs185-dev_1.8.5*.deb \
    && cd /usr/src && mkdir couchdb \
    && wget -qO- https://archive.apache.org/dist/couchdb/source/3.2.1/apache-couchdb-3.2.1.tar.gz | tar zx -C couchdb --strip-components=1 \
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
    } | tee /opt/couchdb/etc/local.d/local.ini

# Add configuration
COPY ./docker-entrypoint.sh /

RUN chmod +x /docker-entrypoint.sh

WORKDIR /opt/couchdb
EXPOSE 5984 4369 9100

ENTRYPOINT ["/docker-entrypoint.sh"]
CMD ["/opt/couchdb/bin/couchdb"]