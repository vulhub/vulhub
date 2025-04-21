# vim:set ft=dockerfile:
FROM alpine:3.5

# alpine includes "postgres" user/group in base install
#   /etc/passwd:22:postgres:x:70:70::/var/lib/postgresql:/bin/sh
#   /etc/group:34:postgres:x:70:
# the home directory for the postgres user, however, is not created by default
# see https://github.com/docker-library/postgres/issues/274
RUN set -ex; \
	postgresHome="$(getent passwd postgres)"; \
	postgresHome="$(echo "$postgresHome" | cut -d: -f6)"; \
	[ "$postgresHome" = '/var/lib/postgresql' ]; \
	mkdir -p "$postgresHome"; \
	chown -R postgres:postgres "$postgresHome"

# su-exec (gosu-compatible) is installed further down

# make the "en_US.UTF-8" locale so postgres will be utf-8 enabled by default
# alpine doesn't require explicit locale-file generation
ENV LANG en_US.utf8

RUN mkdir /docker-entrypoint-initdb.d

ENV PG_MAJOR 9.6
ENV PG_VERSION 9.6.7
ENV PG_SHA256 2ebe3df3c1d1eab78023bdc3ffa55a154aa84300416b075ef996598d78a624c6

RUN set -ex \
	\
	&& apk add --no-cache --virtual .fetch-deps \
		ca-certificates \
		openssl \
		tar \
	\
	&& wget -O postgresql.tar.bz2 "https://ftp.postgresql.org/pub/source/v$PG_VERSION/postgresql-$PG_VERSION.tar.bz2" \
	&& echo "$PG_SHA256 *postgresql.tar.bz2" | sha256sum -c - \
	&& mkdir -p /usr/src/postgresql \
	&& tar \
		--extract \
		--file postgresql.tar.bz2 \
		--directory /usr/src/postgresql \
		--strip-components 1 \
	&& rm postgresql.tar.bz2 \
	\
	&& apk add --no-cache --virtual .build-deps \
		bison \
		coreutils \
		dpkg-dev dpkg \
		flex \
		gcc \
#		krb5-dev \
		libc-dev \
		libedit-dev \
		libxml2-dev \
		libxslt-dev \
		make \
#		openldap-dev \
		openssl-dev \
# configure: error: prove not found
		perl \
# configure: error: Perl module IPC::Run is required to run TAP tests
		perl-ipc-run \
#		perl-dev \
#		python-dev \
#		python3-dev \
#		tcl-dev \
		util-linux-dev \
		zlib-dev \
	\
	&& cd /usr/src/postgresql \
# update "DEFAULT_PGSOCKET_DIR" to "/var/run/postgresql" (matching Debian)
# see https://anonscm.debian.org/git/pkg-postgresql/postgresql.git/tree/debian/patches/51-default-sockets-in-var.patch?id=8b539fcb3e093a521c095e70bdfa76887217b89f
	&& awk '$1 == "#define" && $2 == "DEFAULT_PGSOCKET_DIR" && $3 == "\"/tmp\"" { $3 = "\"/var/run/postgresql\""; print; next } { print }' src/include/pg_config_manual.h > src/include/pg_config_manual.h.new \
	&& grep '/var/run/postgresql' src/include/pg_config_manual.h.new \
	&& mv src/include/pg_config_manual.h.new src/include/pg_config_manual.h \
	&& gnuArch="$(dpkg-architecture --query DEB_BUILD_GNU_TYPE)" \
# explicitly update autoconf config.guess and config.sub so they support more arches/libcs
	&& wget -O config/config.guess 'https://git.savannah.gnu.org/cgit/config.git/plain/config.guess?id=7d3d27baf8107b630586c962c057e22149653deb' \
	&& wget -O config/config.sub 'https://git.savannah.gnu.org/cgit/config.git/plain/config.sub?id=7d3d27baf8107b630586c962c057e22149653deb' \
# configure options taken from:
# https://anonscm.debian.org/cgit/pkg-postgresql/postgresql.git/tree/debian/rules?h=9.5
	&& ./configure \
		--build="$gnuArch" \
# "/usr/src/postgresql/src/backend/access/common/tupconvert.c:105: undefined reference to `libintl_gettext'"
#		--enable-nls \
		--enable-integer-datetimes \
		--enable-thread-safety \
		--enable-tap-tests \
# skip debugging info -- we want tiny size instead
#		--enable-debug \
		--disable-rpath \
		--with-uuid=e2fs \
		--with-gnu-ld \
		--with-pgport=5432 \
		--with-system-tzdata=/usr/share/zoneinfo \
		--prefix=/usr/local \
		--with-includes=/usr/local/include \
		--with-libraries=/usr/local/lib \
		\
# these make our image abnormally large (at least 100MB larger), which seems uncouth for an "Alpine" (ie, "small") variant :)
#		--with-krb5 \
#		--with-gssapi \
#		--with-ldap \
#		--with-tcl \
#		--with-perl \
#		--with-python \
#		--with-pam \
		--with-openssl \
		--with-libxml \
		--with-libxslt \
	&& make -j "$(nproc)" world \
	&& make install-world \
	&& make -C contrib install \
	\
	&& runDeps="$( \
		scanelf --needed --nobanner --format '%n#p' --recursive /usr/local \
			| tr ',' '\n' \
			| sort -u \
			| awk 'system("[ -e /usr/local/lib/" $1 " ]") == 0 { next } { print "so:" $1 }' \
	)" \
	&& apk add --no-cache --virtual .postgresql-rundeps \
		$runDeps \
		bash \
		su-exec \
# tzdata is optional, but only adds around 1Mb to image size and is recommended by Django documentation:
# https://docs.djangoproject.com/en/1.10/ref/databases/#optimizing-postgresql-s-configuration
		tzdata \
	&& apk del .fetch-deps .build-deps \
	&& cd / \
	&& rm -rf \
		/usr/src/postgresql \
		/usr/local/share/doc \
		/usr/local/share/man \
	&& find /usr/local -name '*.a' -delete

# make the sample config easier to munge (and "correct by default")
RUN sed -ri "s!^#?(listen_addresses)\s*=\s*\S+.*!\1 = '*'!" /usr/local/share/postgresql/postgresql.conf.sample

RUN mkdir -p /var/run/postgresql && chown -R postgres:postgres /var/run/postgresql && chmod 2777 /var/run/postgresql

ENV PGDATA /var/lib/postgresql/data
RUN mkdir -p "$PGDATA" && chown -R postgres:postgres "$PGDATA" && chmod 777 "$PGDATA" # this 777 will be replaced by 700 at runtime (allows semi-arbitrary "--user" values)
VOLUME /var/lib/postgresql/data

COPY docker-entrypoint.sh /usr/local/bin/
RUN ln -s usr/local/bin/docker-entrypoint.sh / # backwards compat
ENTRYPOINT ["docker-entrypoint.sh"]

EXPOSE 5432
CMD ["postgres"]