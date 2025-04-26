FROM vulhub/ghostscript:9.56.1

LABEL maintainer="phith0n <root@leavesongs.com>"

ARG IM_VERSION=7.1.0-49
RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends automake autoconf libtool wget ca-certificates xz-utils gcc g++ make \
    && savedAptMark="$(apt-mark showmanual)" \
    && apt-get install -y --no-install-recommends libpng-dev libjpeg62-turbo-dev libfontconfig1-dev libfreetype6-dev \
        librsvg2-dev libxml2-dev zlib1g-dev libgif-dev libltdl-dev \
    && wget -qO- https://github.com/ImageMagick/ImageMagick/archive/${IM_VERSION}.tar.gz \
        | tar xz --strip-components=1 -C /usr/src \
    && cd /usr/src \
    && ./configure --prefix=/usr --with-gslib --disable-dependency-tracking \
    && make \
    && make install \
    && ldconfig /usr/lib \
    # clean
    && apt-mark auto '.*' > /dev/null \
	&& apt-mark manual $savedAptMark \
	&& ldd /usr/bin/* \
		| awk '/=>/ { print $3 }' \
		| sort -u \
		| xargs -r dpkg-query -S \
		| cut -d: -f1 \
		| sort -u \
		| xargs -rt apt-mark manual \
	\
	&& apt-get purge -y --auto-remove -o APT::AutoRemove::RecommendsImportant=false \
	&& rm -rf /var/lib/apt/lists/* /usr/src/*
