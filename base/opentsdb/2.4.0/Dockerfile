FROM openjdk:8-jre

LABEL maintainer="phithon <root@leavesongs.com>"

# install hbase
RUN set -ex \
    && mkdir /opt/hbase \
    && wget -qO- http://www.apache.org/dist/hbase/2.4.5/hbase-2.4.5-bin.tar.gz | tar xz --strip-components 1 -C /opt/hbase

RUN set -ex \
    && cd /tmp \
    && wget https://github.com/OpenTSDB/opentsdb/releases/download/v2.4.0/opentsdb-2.4.0_all.deb \
    && dpkg -i opentsdb-2.4.0_all.deb \
    && mkdir /usr/share/opentsdb/cache

COPY hbase-site.xml /opt/hbase/conf/hbase-site.xml
COPY docker-entrypoint.sh /docker-entrypoint.sh
RUN set -ex \
    && chmod +x /docker-entrypoint.sh

WORKDIR /usr/share/opentsdb

ENTRYPOINT ["/docker-entrypoint.sh"]
