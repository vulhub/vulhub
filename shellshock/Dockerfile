FROM httpd
MAINTAINER Medici.Yan@Gmail.com

WORKDIR /tmp

ENV BUILD_TOOLS \
    wget \
    build-essential \
    bison \
    autoconf

RUN apt-get update \
    && apt-get -y install $BUILD_TOOLS

RUN mkdir -p /usr/local/src/cve-2014-6271 \
    && wget -qO- https://ftp.gnu.org/gnu/bash/bash-4.3.tar.gz \
        | tar zx -C /usr/local/src/cve-2014-6271 --strip-components=1 \
    && cd /usr/local/src/cve-2014-6271 \
    && ./configure --prefix=/usr/local/bash-4.3.0 \
    && make \
    && make install \
    && cd /tmp/ \
    && rm -rf /usr/local/src/cve-2014-6271

RUN mkdir -p /usr/local/src/cve-2014-7169 \
    && wget -qO- https://ftp.gnu.org/gnu/bash/bash-4.3.tar.gz \
        | tar zx -C /usr/local/src/cve-2014-7169 --strip-components=1 \
    && cd /usr/local/src/cve-2014-7169 \
    && wget -qO- $(seq -f "https://ftp.gnu.org/gnu/bash/bash-4.3-patches/bash43-%03g" 25) | patch -p0 \
    && ./configure --prefix=/usr/local/bash-4.3.25 \
    && make \
    && make install \
    && cd /tmp/ \
    && rm -rf /usr/local/src/cve-2014-7169

RUN apt-get purge -y --auto-remove $BUILD_TOOLS