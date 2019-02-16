FROM vulhub/ghostscript:9.26

LABEL maintainer="phith0n <root@leavesongs.com>"

RUN apt-get update \
    && savedAptMark="$(apt-mark showmanual)" \
    && apt-get install -y wget gcc make \
    && wget -qO- https://github.com/ImageMagick/ImageMagick/archive/7.0.8-27.tar.gz \
     | tar xz -C /tmp/ \
    && cd /tmp/ImageMagick-7.0.8-27 \
    && ./configure && make \
    && make install \
    && ldconfig /usr/local/lib \
# reset apt-mark's "manual" list so that "purge --auto-remove" will remove all build dependencies
    && apt-mark auto '.*' > /dev/null \
	&& apt-mark manual $savedAptMark \
	&& ldd /usr/local/bin/* \
		| awk '/=>/ { print $3 }' \
		| sort -u \
		| xargs -r dpkg-query -S \
		| cut -d: -f1 \
		| sort -u \
		| xargs -rt apt-mark manual \
	\
	&& apt-get purge -y --auto-remove -o APT::AutoRemove::RecommendsImportant=false \
	&& rm -rf /var/lib/apt/lists/* /tmp/ImageMagick*
