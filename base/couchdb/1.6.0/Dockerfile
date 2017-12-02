FROM debian:jessie

MAINTAINER phithon <root@leavesongs.com>

COPY ./docker-entrypoint.sh /

RUN apt-get update -y && apt-get install -y --no-install-recommends \
    ca-certificates \
    curl \
    erlang-nox \
    libicu52 \
    libmozjs185-1.0 \
    libnspr4 \
    libnspr4-0d \
    wget \
  && rm -rf /var/lib/apt/lists/*

RUN buildDeps=' \
    gcc \
    g++ \
    erlang-dev \
    libcurl4-openssl-dev \
    libicu-dev \
    libmozjs185-dev \
    libnspr4-dev \
    make \
  ' \
    && apt-get update \
    && apt-get install -y --no-install-recommends $buildDeps \
    && cd /usr/src && mkdir couchdb \
    && wget -qO- https://archive.apache.org/dist/couchdb/source/1.6.0/apache-couchdb-1.6.0.tar.gz | tar zx -C couchdb --strip-components=1 \
    && cd couchdb \
    && ./configure --with-js-lib=/usr/lib --with-js-include=/usr/include/mozjs \
    && make && make install \
    && apt-get purge -y --auto-remove $buildDeps \
    && rm -rf /var/lib/apt/lists/* /usr/src/couchdb \
    && mkdir -p /var/lib/couchdb \
    && sed -e 's/^bind_address = .*$/bind_address = 0.0.0.0/' -i /usr/local/etc/couchdb/default.ini \
    && sed -e 's!/usr/local/var/log/couchdb/couch.log$!/dev/null!' -i /usr/local/etc/couchdb/default.ini \
    && chmod +x /docker-entrypoint.sh

WORKDIR /var/lib/couchdb
EXPOSE 5984

ENTRYPOINT ["/docker-entrypoint.sh"]
CMD ["couchdb"]