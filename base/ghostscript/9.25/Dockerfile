FROM debian:stretch

LABEL maintainer="phith0n <root@leavesongs.com>"

ADD https://github.com/ArtifexSoftware/ghostpdl-downloads/releases/download/gs925/ghostscript-9.25-linux-x86_64.tgz /tmp/ghostscript.tgz

RUN set -ex \
    && tar zxf /tmp/ghostscript.tgz --strip-components 1 -C /tmp/ \
    && mkdir -p /opt/ghostscript \
    && mv /tmp/gs-* /usr/local/bin/gs \
    && rm -rf /tmp/*
