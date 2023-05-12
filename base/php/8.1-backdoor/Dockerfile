FROM debian:buster

LABEL maintainer="phithon <root@leavesongs.com>"

ENV PHP_INI_DIR /usr/local/etc/php
ENV PHPIZE_DEPS \
        autoconf \
        dpkg-dev \
        file \
        g++ \
        gcc \
        libc-dev \
        make \
        pkg-config \
        re2c \
        bison
ARG PHP_URL="https://github.com/php/php-src/archive/c730aa26bd52829a49f2ad284b181b7e82a68d7d.tar.gz"

RUN set -eux \
    && mkdir -p $PHP_INI_DIR/conf.d \
    && savedAptMark="$(apt-mark showmanual)" \
    && apt-get update \
    && apt-get install -y --no-install-recommends \
        ${PHPIZE_DEPS} \
        \
        ca-certificates \
        curl \
        wget \
        libargon2-dev \
        libcurl4-openssl-dev \
        libedit-dev \
        libonig-dev \
        libsodium-dev \
        libsqlite3-dev \
        libssl-dev \
        libxml2-dev \
        zlib1g-dev \
    \
    && mkdir -p /usr/src \
    && cd /usr/src \
    && curl -#SL ${PHP_URL} | tar xz --strip-components=1 \
    && gnuArch="$(dpkg-architecture --query DEB_BUILD_GNU_TYPE)" \
    && debMultiarch="$(dpkg-architecture --query DEB_BUILD_MULTIARCH)" \
    && if [ ! -d /usr/include/curl ]; then \
        ln -sT "/usr/include/$debMultiarch/curl" /usr/local/include/curl; \
    fi \
    && ./buildconf \
    && ./configure \
        --build="$gnuArch" \
        --with-config-file-path="$PHP_INI_DIR" \
        --with-config-file-scan-dir="$PHP_INI_DIR/conf.d" \
        \
# make sure invalid --configure-flags are fatal errors instead of just warnings
        --enable-option-checking=fatal \
        \
# https://github.com/docker-library/php/issues/439
        --with-mhash \
        \
# https://github.com/docker-library/php/issues/822
        --with-pic \
        \
# --enable-ftp is included here because ftp_ssl_connect() needs ftp to be compiled statically (see https://github.com/docker-library/php/issues/236)
        --enable-ftp \
# --enable-mbstring is included here because otherwise there's no way to get pecl to use it properly (see https://github.com/docker-library/php/issues/195)
        --enable-mbstring \
# --enable-mysqlnd is included here because it's harder to compile after the fact than extensions are (since it's a plugin for several extensions, not an extension in itself)
        --enable-mysqlnd \
# https://wiki.php.net/rfc/argon2_password_hash (7.2+)
        \
        --with-password-argon2 \
# https://wiki.php.net/rfc/libsodium
        --with-sodium=shared \
# always build against system sqlite3 (https://github.com/php/php-src/commit/6083a387a81dbbd66d6316a3a12a63f06d5f7109)
        --with-pdo-sqlite=/usr \
        --with-sqlite3=/usr \
        \
        --with-curl \
        --with-libedit \
        --with-openssl \
        --with-zlib \
        \
# bundled pcre does not support JIT on s390x
# https://manpages.debian.org/stretch/libpcre3-dev/pcrejit.3.en.html#AVAILABILITY_OF_JIT_SUPPORT
        $(test "$gnuArch" = 's390x-linux-gnu' && echo '--without-pcre-jit') \
        --with-libdir="lib/$debMultiarch" \
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