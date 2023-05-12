FROM buildpack-deps:stretch-scm

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && BUILDDEP="gcc g++ make pkg-config cmake xz-utils" \
    && apt-get update \
    && apt-get install --no-install-recommends -y \
        ca-certificates \
        wget \
        libc6-dev \
        zlib1g-dev \
        libgcrypt20-dev \
        libgpg-error-dev \
        libkrb5-dev \
        $BUILDDEP \
    && wget -qO- https://www.libssh.org/files/0.7/libssh-0.7.4.tar.xz \
        | xz -c -d | tar x -C /usr/src --strip-components=1 \
    && mkdir -p /usr/src/build \
    && cd /usr/src/build \
    && cmake \
        -DCMAKE_INSTALL_PREFIX=/usr \
        -DWITH_SERVER=ON \
        -DWITH_STATIC_LIB=ON \
        -DWITH_GSSAPI=ON \
        -DWITH_GCRYPT=ON \
        -DWITH_SFTP=ON \
        -DWITH_EXAMPLES=ON \
        -DWITH_NACL=OFF \
        -DGSSAPI_LIBRARIES=/usr/lib/x86_64-linux-gnu \
        -DGSSAPI_INCLUDE_DIR=/usr \
        .. \
    && make && make install \
    && apt-get purge -y --auto-remove $BUILDDEP

CMD [ "/usr/src/build/examples/samplesshd-cb" ]


