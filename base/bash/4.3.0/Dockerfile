FROM debian:jessie

LABEL maintainer="phithon <root@leavesongs.com>"

RUN apt-get update \
    && savedAptMark="$(apt-mark showmanual)" \
    && apt-get install -y wget \
        build-essential \
        bison \
        autoconf \
    && mkdir /tmp/bash \
    && wget -qO- https://ftp.gnu.org/gnu/bash/bash-4.3.tar.gz \
        | tar zx -C /tmp/bash --strip-components=1 \
    && cd /tmp/bash \
    && ./configure --prefix=/usr/local/bash-4.3.0 \
    && make \
    && make install \
# reset apt-mark's "manual" list so that "purge --auto-remove" will remove all build dependencies
    && apt-mark auto '.*' > /dev/null \
	&& apt-mark manual $savedAptMark \
	&& ldd /usr/local/bash-4.3.0/bin/* \
		| awk '/=>/ { print $3 }' \
		| sort -u \
		| xargs -r dpkg-query -S \
		| cut -d: -f1 \
		| sort -u \
		| xargs -rt apt-mark manual \
	\
	&& apt-get purge -y --auto-remove -o APT::AutoRemove::RecommendsImportant=false \
    && rm -rf /var/lib/apt/lists/* /tmp/bash

ENV PATH /usr/local/bash-4.3.0/bin:${PATH}
