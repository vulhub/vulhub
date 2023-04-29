FROM openjdk:8-jre

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends python3 libfindbin-libs-perl

RUN set -ex \
    && mkdir -p /opt/druid \
    && wget -qO- https://archive.apache.org/dist/druid/0.20.0/apache-druid-0.20.0-bin.tar.gz | tar xz --strip-components 1 -C /opt/druid

EXPOSE 8888
WORKDIR /opt/druid
CMD [ "/opt/druid/bin/start-nano-quickstart" ]
