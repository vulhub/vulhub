FROM debian:bullseye

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends wget ca-certificates gcc make rustc cargo automake autoconf libtool libgdk-pixbuf2.0-dev libgirepository1.0-dev \
        libxml2-dev libcairo2-dev libpango1.0-dev \
    && PATH="$PATH:/usr/lib/x86_64-linux-gnu/gdk-pixbuf-2.0" \
    && wget -qO- https://download.gnome.org/sources/librsvg/2.50/librsvg-2.50.7.tar.xz | tar xJ --strip-components 1 -C /usr/src \
    && cd /usr/src \
    && ./configure --prefix=/usr \
    && make \
    && make install \
    && apt-get purge -y --autoremove gcc make rustc cargo automake autoconf libtool \
    && ldconfig /usr/lib \
    && rm -rf /var/lib/apt/lists/* /usr/src/*
