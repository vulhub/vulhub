FROM openjdk:8-jdk

LABEL maintainer="phithon <root@leavesongs.com>"

ENV HBASE_VERSION 2.4.5
ENV HBASE_URL http://www.apache.org/dist/hbase/$HBASE_VERSION/hbase-$HBASE_VERSION-bin.tar.gz

RUN set -ex \
    && mkdir /opt/hbase \
    && wget -qO- ${HBASE_URL} | tar xz --strip-components 1 -C /opt/hbase \
    && 

WORKDIR /opt/hbase
ENV HBASE_MANAGES_ZK false
COPY hbase-site.xml /opt/hbase/conf/hbase-site.xml

CMD ["/opt/hbase/bin/hbase-daemon.sh", "--config", "/opt/hbase/conf", "foreground_start", "master"]
