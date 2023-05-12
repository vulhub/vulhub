FROM debian:buster

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends ca-certificates \
    && savedAptMark="$(apt-mark showmanual)" \
    && apt-get install -y --no-install-recommends gcc make wget dh-autoreconf libcurl4-gnutls-dev libexpat1-dev gettext zlib1g-dev libssl-dev asciidoc xmlto docbook2x \
    && mkdir -p /usr/src/git \
    && wget -qO- https://github.com/git/git/archive/v2.12.2.tar.gz | tar zx -C /usr/src/git --strip-components=1 \
    && cd /usr/src/git \
    && make configure \
    && ./configure --prefix=/usr/local \
    && make all doc info \
    && make install install-doc \
    # remove unused libraries
    && apt-mark auto '.*' > /dev/null \
    && apt-mark manual $savedAptMark \
    && find /usr/local -type f -executable -exec ldd '{}' ';' \
        | awk '/=>/ { print $(NF-1) }' \
        | sort -u \
        | xargs -r dpkg-query --search \
        | cut -d: -f1 \
        | sort -u \
        | xargs -r apt-mark manual \
    \
    && apt-get purge -y --auto-remove -o APT::AutoRemove::RecommendsImportant=false \
    && rm -rf /var/lib/apt/lists/* /usr/src/git
