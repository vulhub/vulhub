FROM php:7.1-apache

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && mkdir /tmp/libxml \
    && cd /tmp/libxml \
    && curl -#sSL http://xmlsoft.org/sources/libxml2-2.8.0.tar.gz | tar xz --strip-components=1 \
    && ./configure && make && make install && make clean \
    && cd / && rm -rf /tmp/libxml

RUN set -ex \
    && apt-get update \
	&& apt-get install -y --no-install-recommends \
		libargon2-0-dev \
		libcurl4-openssl-dev \
		libedit-dev \
		libsqlite3-dev \
		libssl-dev \
		libxml2-dev \
		zlib1g-dev \
		${PHP_EXTRA_BUILD_DEPS:-} \
	\
	&& rm -rf /var/lib/apt/lists/* \
	\
	&& docker-php-source extract \
	&& cd /usr/src/php \
	&& ./configure \
		--with-config-file-path="$PHP_INI_DIR" \
		--with-config-file-scan-dir="$PHP_INI_DIR/conf.d" \
		\
		--disable-cgi \
		\
# --enable-ftp is included here because ftp_ssl_connect() needs ftp to be compiled statically (see https://github.com/docker-library/php/issues/236)
		--enable-ftp \
# --enable-mbstring is included here because otherwise there's no way to get pecl to use it properly (see https://github.com/docker-library/php/issues/195)
		--enable-mbstring \
# --enable-mysqlnd is included here because it's harder to compile after the fact than extensions are (since it's a plugin for several extensions, not an extension in itself)
		--enable-mysqlnd \
		\
		--with-curl \
		--with-libedit \
		--with-openssl \
		--with-zlib \
		\
		$PHP_EXTRA_CONFIGURE_ARGS \
	&& make -j "$(nproc)" \
	&& make install \
	&& { find /usr/local/bin /usr/local/sbin -type f -executable -exec strip --strip-all '{}' + || true; } \
	&& make clean \
	&& docker-php-source delete \
	\
# reset apt-mark's "manual" list so that "purge --auto-remove" will remove all build dependencies
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
	\
# https://github.com/docker-library/php/issues/443
	&& pecl update-channels \
	&& rm -rf /tmp/pear ~/.pearrc
