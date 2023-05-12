FROM openjdk:8-jre

LABEL maintainer="phithon <root@leavesongs.com>"

ARG BASEDIR="/mnt/skywalking"
RUN set -ex \
    && mkdir -p ${BASEDIR} \
    && wget -qO- http://archive.apache.org/dist/skywalking/8.3.0/apache-skywalking-apm-8.3.0.tar.gz | tar xz --strip-components=1 -C ${BASEDIR}

COPY oap-service.sh /mnt/skywalking/bin/
COPY web-server.sh  /mnt/skywalking/bin/

RUN set -ex \
    chmod +x /mnt/skywalking/bin/*.sh

WORKDIR ${BASEDIR}
CMD bin/web-server.sh