# Licensed under the Apache License, Version 2.0 (the "License"); you may not
# use this file except in compliance with the License. You may obtain a copy of
# the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
# License for the specific language governing permissions and limitations under
# the License.

FROM debian:jessie

LABEL maintainer="phithon <root@leavesongs.com>"

# Install instructions from https://cwiki.apache.org/confluence/display/COUCHDB/Debian

RUN groupadd -r couchdb && useradd -d /var/lib/couchdb -g couchdb couchdb

RUN apt-get update -y && apt-get install -y --no-install-recommends \
    ca-certificates \
    curl \
    erlang-nox \
    libicu52 \
    libmozjs185-1.0 \
    libnspr4 \
    libnspr4-0d \
  && rm -rf /var/lib/apt/lists/*

# grab gosu for easy step-down from root and tini for signal handling
RUN curl -o /usr/local/bin/gosu -fSL "https://github.com/tianon/gosu/releases/download/1.7/gosu-$(dpkg --print-architecture)" \
  && chmod +x /usr/local/bin/gosu \
  && curl -o /usr/local/bin/tini -fSL "https://github.com/krallin/tini/releases/download/v0.14.0/tini" \
  && chmod +x /usr/local/bin/tini

# download dependencies, compile and install couchdb,
# set correct permissions, expose couchdb to the outside and disable logging to disk
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
  && apt-get update && apt-get install -y --no-install-recommends $buildDeps \
  && curl -fSL https://archive.apache.org/dist/couchdb/source/1.6.0/apache-couchdb-1.6.0.tar.gz -o couchdb.tar.gz \
  && mkdir -p /usr/src/couchdb \
  && tar -xzf couchdb.tar.gz -C /usr/src/couchdb --strip-components=1 \
  && cd /usr/src/couchdb \
  && ./configure --with-js-lib=/usr/lib --with-js-include=/usr/include/mozjs \
  && make && make install \
  && apt-get purge -y --auto-remove $buildDeps \
  && rm -rf /var/lib/apt/lists/* /usr/src/couchdb /couchdb.tar.gz* \
  && chown -R couchdb:couchdb \
    /usr/local/lib/couchdb /usr/local/etc/couchdb \
    /usr/local/var/lib/couchdb /usr/local/var/log/couchdb /usr/local/var/run/couchdb \
  && chmod -R g+rw \
    /usr/local/lib/couchdb /usr/local/etc/couchdb \
    /usr/local/var/lib/couchdb /usr/local/var/log/couchdb /usr/local/var/run/couchdb \
  && mkdir -p /var/lib/couchdb \
  && sed -e 's/^bind_address = .*$/bind_address = 0.0.0.0/' -i /usr/local/etc/couchdb/default.ini \
  && sed -e 's!/usr/local/var/log/couchdb/couch.log$!/dev/null!' -i /usr/local/etc/couchdb/default.ini

COPY ./docker-entrypoint.sh /
RUN chmod +x /docker-entrypoint.sh

EXPOSE 5984
WORKDIR /var/lib/couchdb

ENTRYPOINT ["tini", "--", "/docker-entrypoint.sh"]
CMD ["couchdb"]