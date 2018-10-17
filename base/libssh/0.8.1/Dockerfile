FROM debian:stretch

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && BUILDDEP="gcc g++ make pkg-config cmake xz-utils" \
    && apt-get update \
    && apt-get install --no-install-recommends -y \
        ca-certificates \
        wget \
        libc6-dev \
        zlib1g-dev \
        libssl-dev \
        libgcrypt20-dev \
        libgpg-error-dev \
        $BUILDDEP \
    && wget -qO- https://www.libssh.org/files/0.8/libssh-0.8.1.tar.xz \
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
        -DWITH_THREADS=ON \
        .. \
    && make && make install \
    && apt-get purge -y --auto-remove $BUILDDEP

CMD [ "/usr/src/build/examples/ssh_server_fork", "--usage" ]