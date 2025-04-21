FROM debian:stretch

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && apt-get update \
    && savedAptMark="$(apt-mark showmanual)" \
    && apt-get install -y wget \
    && apt-get install -y --no-install-recommends build-essential zlib1g-dev libssl1.0-dev \
    && cd /usr/src \
    && wget -qO- https://openbsd.hk/pub/OpenBSD/OpenSSH/portable/openssh-7.7p1.tar.gz | tar zx --strip-components 1 \
    && echo "sshd:x:74:74:Privilege-separated SSH:/usr/local/sbin/sshd:/sbin/nologin" >> /etc/passwd \
    && ./configure \
    && make \
    && make install \
# reset apt-mark's "manual" list so that "purge --auto-remove" will remove all build dependencies 
    && apt-mark auto '.*' > /dev/null \
    && apt-mark manual $savedAptMark \
    && ldd /usr/local/bin/* /usr/local/sbin/* \
        | awk '/=>/ { print $3 }' \
        | sort -u \
        | xargs -r dpkg-query -S \
        | cut -d: -f1 \
        | sort -u \
        | xargs -rt apt-mark manual \
    \
    && apt-get purge -y --auto-remove -o APT::AutoRemove::RecommendsImportant=false \
    && rm -rf /var/lib/apt/lists/* /usr/src/*

COPY --chown=root:root docker-entrypoint.sh /docker-entrypoint.sh

RUN set -ex \
    && echo "PermitRootLogin yes" >> /usr/local/etc/sshd_config \
    && chmod +x /docker-entrypoint.sh

EXPOSE 22

ENTRYPOINT ["/docker-entrypoint.sh"]

CMD ["/usr/local/sbin/sshd", "-D"]