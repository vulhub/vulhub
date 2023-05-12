FROM debian:stretch

LABEL maintainer="phithon <root@leavesongs.com>"

ENV PHP_INI_DIR /usr/local/etc/php

RUN set -ex \
    && mkdir -p $PHP_INI_DIR/conf.d \
    && savedAptMark="$(apt-mark showmanual)" \
    && apt-get update \
    && apt-get install -y --no-install-recommends \
        autoconf \
		dpkg-dev \
		file \
		g++ \
		gcc \
		libc-dev \
		make \
		pkg-config \
		re2c \
        bison \
        \
        libcurl4-openssl-dev \
		libedit-dev \
		libsqlite3-dev \
		libssl-dev \
		libxml2-dev \
		zlib1g-dev \
        \
        curl \
    \
    && mkdir /tmp/libxml \
    && cd /tmp/libxml \
    && curl -#sSL http://xmlsoft.org/sources/libxml2-2.8.0.tar.gz | tar xz --strip-components=1 \
    && ./configure && make && make install && make clean \
    && cd /usr/src && rm -rf /tmp/libxml \
    \
    && curl -#SL http://php.net/get/php-7.0.30.tar.gz/from/this/mirror | tar xz --strip-components=1 \
    && gnuArch="$(dpkg-architecture --query DEB_BUILD_GNU_TYPE)" \
    && debMultiarch="$(dpkg-architecture --query DEB_BUILD_MULTIARCH)" \
    && ./configure --build="$gnuArch" \
		--with-config-file-path="$PHP_INI_DIR" \
		--with-config-file-scan-dir="$PHP_INI_DIR/conf.d" \
		--enable-option-checking=fatal \
		--with-mhash \
        --enable-ftp \
		--enable-mbstring \
		--enable-mysqlnd \
		--with-curl \
		--with-libedit \
		--with-openssl \
		--with-zlib \
		$(test "$gnuArch" = 's390x-linux-gnu' && echo '--without-pcre-jit') \
		--with-libdir="lib/$debMultiarch" \
        --disable-cgi \
    && make -j "$(nproc)" \
	&& make install \
    && find /usr/local/bin /usr/local/sbin -type f -executable -exec strip --strip-all '{}' + || true \
	&& make clean \
	&& cd / \
    && apt-mark auto '.*' > /dev/null \
	&& [ -z "$savedAptMark" ] || apt-mark manual $savedAptMark \
	&& find /usr/local -type f -executable -exec ldd '{}' ';' \
		| awk '/=>/ { print $(NF-1) }' \
		| sort -u \
		| xargs -r dpkg-query --search \
		| cut -d: -f1 \
		| sort -u \
		| xargs -r apt-mark manual \
	\
	&& apt-get purge -y --auto-remove -o APT::AutoRemove::RecommendsImportant=false \
	\
	&& php --version \
    && rm -rf /usr/src/* \
    && mkdir -p /var/www/html

WORKDIR /var/www/html

CMD ["php", "-S", "0.0.0.0:80", "-t", "/var/www/html"]