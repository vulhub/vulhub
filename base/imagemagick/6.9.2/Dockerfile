FROM debian:stretch

ARG IM_VERSION=6.9.2-10
RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends ghostscript \
    && savedAptMark="$(apt-mark showmanual)" \
    && apt-get install -y --no-install-recommends automake autoconf libtool ghostscript libltdl-dev wget ca-certificates xz-utils gcc make libpng-dev libjpeg62-turbo-dev \
        libfontconfig1-dev libfreetype6-dev librsvg2-dev libxml2-dev zlib1g-dev libgif-dev \
    && wget -qO- https://www.imagemagick.org/download/releases/ImageMagick-${IM_VERSION}.tar.xz \
     | tar xJ --strip-components=1 -C /usr/src \
    && cd /usr/src \
    && ./configure --prefix=/usr --with-gslib --with-gs-font-dir=/usr/share/ghostscript/fonts/ --disable-dependency-tracking \
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
