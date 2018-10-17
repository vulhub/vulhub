FROM buildpack-deps:wheezy-scm

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && apt-get update \
    && apt-get install --no-install-recommends -y \
        build-essential \
        wget \
        cmake \
        libssl-dev \
    \
    && wget --no-check-certificate -qO- https://git.libssh.org/projects/libssh.git/snapshot/v0-6.tar.gz \
        | tar zx -C /usr/src --strip-components=1 \
    && cd /usr/src/build \
    && cmake ../ -DCMAKE_INSTALL_PREFIX=/usr/local -DCMAKE_BUILD_TYPE=Release -DWITH_SERVER=ON \
    && make && make install \
    && apt-get purge -y --auto-remove build-essential wget cmake gcc
