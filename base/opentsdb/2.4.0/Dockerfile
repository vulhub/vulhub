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

CMD java -enableassertions -enablesystemassertions -classpath lib/asm-4.0.jar:lib/async-1.4.0.jar:lib/asynchbase-1.8.2.jar:lib/commons-jexl-2.1.1.jar:lib/commons-logging-1.1.1.jar:lib/commons-math3-3.4.1.jar:lib/guava-18.0.jar:lib/jackson-annotations-2.9.5.jar:lib/jackson-core-2.9.5.jar:lib/jackson-databind-2.9.5.jar:lib/javacc-6.1.2.jar:lib/jgrapht-core-0.9.1.jar:lib/kryo-2.21.1.jar:lib/log4j-over-slf4j-1.7.7.jar:lib/logback-classic-1.0.13.jar:lib/logback-core-1.0.13.jar:lib/minlog-1.2.jar:lib/netty-3.10.6.Final.jar:lib/protobuf-java-2.5.0.jar:lib/reflectasm-1.07-shaded.jar:lib/slf4j-api-1.7.7.jar:lib/tsdb-2.4.0.jar:lib/zookeeper-3.4.6.jar net.opentsdb.tools.TSDMain --config=/etc/opentsdb/opentsdb.conf --staticroot=${WORKDIR}/static --cachedir=${WORKDIR}/cache --port=4242 --zkquorum=${ZKQUORUM} --zkbasedir=/hbase --read-only --disable-ui
