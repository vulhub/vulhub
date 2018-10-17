FROM golang:1.11

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && apt-get update \
    && apt-get install --no-install-recommends -y \
        cmake \
        xz-utils \
        zlib1g-dev \
        libssl-dev \
        libgcrypt20-dev \
        libgpg-error-dev \
    && wget -qO- https://git.libssh.org/projects/libssh.git/snapshot/libssh-e37fd83254f32866c4205f61e5cce5f26a08a030.tar.gz | tar zx -C /usr/src --strip-components=1 \
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
    && make && make install

RUN CGO_LDFLAGS="-L/usr/lib -lssh -lgcrypt -lgpg-error -lz" \
    && CGO_CFLAGS="-I/usr/include" \
    && PKG_CONFIG_PATH="/usr/src/build" \
    && go get github.com/karfield/ssh2go
