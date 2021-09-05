FROM openjdk:8-jre

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && cd /tmp \
    && wget https://github.com/OpenTSDB/opentsdb/releases/download/v2.4.0/opentsdb-2.4.0_all.deb \
    && dpkg -i opentsdb-2.4.0_all.deb \
    && mkdir /usr/share/opentsdb/cache

WORKDIR /usr/share/opentsdb
ENV WORKDIR /usr/share/opentsdb
ENV ZKQUORUM zookeeper:2181

CMD ["java", "-enableassertions", "-enablesystemassertions", "-classpath", "$WORKDIR/libs/*", "net.opentsdb.tools.TSDMain", "--config=/etc/opentsdb/opentsdb.conf", "--staticroot=${WORKDIR}/static", "--cachedir=${WORKDIR}/cache", "--port=4242", "--zkquorum=${ZKQUORUM}", "--zkbasedir=/hbase", "--read-only", "--disable-ui"]
