FROM debian:jessie

LABEL maintainer="phith0n <root@leavesongs.com>"

ADD https://github.com/ArtifexSoftware/ghostpdl-downloads/releases/download/gs921/ghostscript-9.21-linux-x86_64.tgz /tmp/ghostscript.tgz

RUN set -ex \
    && tar zx --strip-components 1 -C /tmp/ /tmp/ghostscript.tgz \
    && mkdir -p /opt/ghostscript \
    && mv /tmp/gs-* /usr/local/bin/gs \
    && rm -rf /tmp/*
