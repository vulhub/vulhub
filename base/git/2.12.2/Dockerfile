FROM vulhub/ubuntu:trusty

MAINTAINER phithon <root@leavesongs.com>

ENV BUILD_TOOLS gcc \
                make \
                wget \
                dh-autoreconf \
                libcurl4-gnutls-dev \
                libexpat1-dev \
                gettext \
                libz-dev \
                libssl-dev \
                asciidoc \
                xmlto \
                docbook2x

RUN apt-get update \
    && apt-get install -y $BUILD_TOOLS \
    && rm -r /var/lib/apt/lists/*

RUN mkdir -p /usr/src/git \
    && wget -qO- https://github.com/git/git/archive/v2.12.2.tar.gz | tar zx -C /usr/src/git --strip-components=1 \
    && cd /usr/src/git \
    && make configure \
    && ./configure --prefix=/usr \
    && make all doc info \
    && make install install-doc \
    && make clean \
    && cd / \
    && rm -rf /usr/src/git

RUN apt-get purge -y --auto-remove -o APT::AutoRemove::RecommendsImportant=false $BUILD_TOOLS