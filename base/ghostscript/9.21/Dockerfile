FROM debian:jessie

LABEL maintainer="phith0n <root@leavesongs.com>"

ARG GHOSTSCRIPT_VERSION=9.21

RUN set -ex \
    && apt-get update \
    && apt-get install --no-install-recommends -y wget  \
    && wget -qO- https://github.com/ArtifexSoftware/ghostpdl-downloads/releases/download/gs921/ghostscript-${GHOSTSCRIPT_VERSION}-linux-x86_64.tgz \
        | tar zx --strip-components 1 -C /tmp/ \
    && mkdir -p /opt/ghostscript \
    && mv /tmp/gs-* /usr/local/bin/gs \
    && apt-get purge --auto-remove -y wget \
    && rm -rf /var/lib/apt/lists/* /tmp/*
