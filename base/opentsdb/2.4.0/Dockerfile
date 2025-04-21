FROM openjdk:8-jdk

LABEL maintainer="phithon <root@leavesongs.com>"

# install hbase
RUN set -ex \
    && mkdir /opt/hbase \
    && wget -qO- https://archive.apache.org/dist/hbase/2.4.5/hbase-2.4.5-bin.tar.gz | tar xz --strip-components 1 -C /opt/hbase

# install opentsdb
RUN set -ex \
    && cd /tmp \
    && wget https://github.com/OpenTSDB/opentsdb/releases/download/v2.4.0/opentsdb-2.4.0_all.deb \
    && dpkg -i opentsdb-2.4.0_all.deb \
    && echo "tsd.core.auto_create_metrics = true" >> /etc/opentsdb/opentsdb.conf \
    && mkdir /usr/share/opentsdb/cache \
    && rm -rf /tmp/opentsdb-2.4.0_all.deb

# install 
RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends gnuplot-nox \
    && rm -rf /var/lib/apt/lists/*

COPY docker-entrypoint.sh /docker-entrypoint.sh
RUN set -ex \
    && chmod +x /docker-entrypoint.sh

WORKDIR /usr/share/opentsdb

ENTRYPOINT ["/docker-entrypoint.sh"]
