FROM php:7.4

LABEL maintainer="phith0n <root@leavesongs.com>"

ADD https://github.com/ArtifexSoftware/ghostpdl-downloads/releases/download/gs9561/ghostscript-9.56.1-linux-x86_64.tgz /tmp/ghostscript.tgz

RUN set -ex \
    && tar zxf /tmp/ghostscript.tgz --strip-components 1 -C /tmp/ \
    && mkdir -p /opt/ghostscript \
    && mv /tmp/gs-* /usr/local/bin/gs \
    && rm -rf /tmp/*

RUN set -ex \
    && { \
        echo 'deb [check-valid-until=no] http://snapshot.debian.org/archive/debian/20230630/ bullseye main'; \
        # echo 'deb-src [check-valid-until=no] http://snapshot.debian.org/archive/debian/20230630/ bullseye main'; \
        echo 'deb [check-valid-until=no] http://snapshot.debian.org/archive/debian-security/20230630/ bullseye-security main'; \
        # echo 'deb-src [check-valid-until=no] http://snapshot.debian.org/archive/debian-security/20230630/ bullseye-security main'; \
        echo 'deb [check-valid-until=no] http://snapshot.debian.org/archive/debian/20230630/ bullseye-updates main'; \
        # echo 'deb-src [check-valid-until=no] http://snapshot.debian.org/archive/debian/20230630/ bullseye-updates main'; \
    } > /etc/apt/sources.list \
    && apt-get update \
    && apt-get install -y --no-install-recommends librsvg2-dev librsvg2-bin wget ca-certificates libpng-dev libjpeg62-turbo-dev libfontconfig1-dev zlib1g-dev \
        libgif-dev libltdl-dev libxml2-dev libfreetype6-dev

ARG IM_VERSION=7.1.1-17
RUN set -ex \
    && wget -qO- https://github.com/ImageMagick/ImageMagick/archive/${IM_VERSION}.tar.gz \
        | tar xz --strip-components=1 -C /usr/local/src \
    && cd /usr/local/src \
    && ./configure --prefix=/usr --with-gslib --with-rsvg --disable-dependency-tracking \
    && make \
    && make install \
    && ldconfig /usr/lib \
	&& rm -rf /var/lib/apt/lists/* /usr/local/src/*

RUN set -ex \
    && pecl install imagick-3.7.0 \
    && echo 'extension=imagick.so' > /usr/local/etc/php/conf.d/imagick.ini

RUN set -ex \
    && sed -i 's|</policymap>|  <policy domain="system" name="font" value="/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"/>\n</policymap>|' \
        /usr/etc/ImageMagick-7/policy.xml
