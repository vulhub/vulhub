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
  && curl -o /usr/bin/gosu -fSL "https://github.com/tianon/gosu/releases/download/1.7/gosu-$(dpkg --print-architecture)" \
  && chmod +x /usr/bin/gosu \
  && curl -o /usr/bin/tini -fSL "https://github.com/krallin/tini/releases/download/v0.14.0/tini" \
  && chmod +x /usr/bin/tini \
  && rm -rf /var/lib/apt/lists/* \
  && groupadd -r couchdb && useradd -d /var/lib/couchdb -g couchdb couchdb

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
    && ./configure --prefix=/usr --with-js-lib=/usr/lib --with-js-include=/usr/include/mozjs \
    && make && make install \
    && apt-get purge -y --auto-remove $buildDeps \
    && rm -rf /var/lib/apt/lists/* /usr/src/couchdb \
    && chown -R couchdb:couchdb \
        /usr/lib/couchdb /usr/etc/couchdb \
        /usr/var/lib/couchdb /usr/var/log/couchdb /usr/var/run/couchdb \
    && chmod -R g+rw \
        /usr/lib/couchdb /usr/etc/couchdb \
        /usr/var/lib/couchdb /usr/var/log/couchdb /usr/var/run/couchdb \
    && mkdir -p /var/lib/couchdb \
    && sed -e 's/^bind_address = .*$/bind_address = 0.0.0.0/' -i /usr/etc/couchdb/default.ini \
    && sed -e 's!/usr/var/log/couchdb/couch.log$!/dev/null!' -i /usr/etc/couchdb/default.ini \
    && chmod +x /docker-entrypoint.sh

WORKDIR /var/lib/couchdb
EXPOSE 5984

ENTRYPOINT ["tini", "--", "/docker-entrypoint.sh"]
CMD ["couchdb"]