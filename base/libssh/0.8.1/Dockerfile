FROM buildpack-deps:stretch-scm

LABEL maintainer="phithon <root@leavesongs.com>"

COPY ssh_server_fork.patch /ssh_server_fork.patch

RUN set -ex \
    && BUILDDEP="gcc g++ make pkg-config cmake xz-utils patch" \
    && apt-get update \
    && apt-get install --no-install-recommends -y \
        ca-certificates \
        wget \
        libc6-dev \
        zlib1g-dev \
        libgcrypt20-dev \
        libgpg-error-dev \
        $BUILDDEP \
    && wget -qO- https://www.libssh.org/files/0.8/libssh-0.8.1.tar.xz \
        | xz -c -d | tar x -C /usr/src --strip-components=1 \
    && mkdir -p /usr/src/build \
    && patch /usr/src/examples/ssh_server_fork.c < /ssh_server_fork.patch \
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

RUN ssh-keygen -t ecdsa -f /etc/ssh/ssh_host_ecdsa_key -q -N "" \
    && ssh-keygen -t dsa -f /etc/ssh/ssh_host_dsa_key -q -N "" \
    && ssh-keygen -t rsa -b 2048 -f /etc/ssh/ssh_host_rsa_key -q -N ""

CMD /usr/src/build/examples/ssh_server_fork --hostkey=/etc/ssh/ssh_host_rsa_key --ecdsakey=/etc/ssh/ssh_host_ecdsa_key --dsakey=/etc/ssh/ssh_host_dsa_key --rsakey=/etc/ssh/ssh_host_rsa_key -p 22 0.0.0.0
