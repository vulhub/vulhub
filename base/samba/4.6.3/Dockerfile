FROM ubuntu:16.04

LABEL maintainer="phithon <root@leavesongs.com>"

ENV BUILD_DEP wget build-essential autoconf 

RUN set -ex \
    && echo "krb5-config krb5-config/default_realm string " | debconf-set-selections \
    && apt-get update \
    && apt-get install -y $BUILD_DEP acl attr bison  \
  debhelper dnsutils docbook-xml docbook-xsl flex gdb krb5-user \
  libacl1-dev libaio-dev libattr1-dev libblkid-dev libbsd-dev \
  libcap-dev libcups2-dev libgnutls28-dev libjson-perl \
  libldap2-dev libncurses5-dev libpam0g-dev libparse-yapp-perl \
  libpopt-dev libreadline-dev pkg-config perl perl-modules \
  python-all-dev python-dev python-dnspython python-crypto \
  xsltproc zlib1g-dev libsystemd-dev libgpgme11-dev python-gpgme python-m2crypto \
    && mkdir -p /usr/src/samba \
    && wget -qO- https://download.samba.org/pub/samba/stable/samba-4.6.3.tar.gz \
        | tar zx -C /usr/src/samba --strip-components=1 \
    && cd /usr/src/samba \
    && ./configure \
    && make \
    && make install \
    && make clean \
    && cd / \
    && rm -rf /usr/src/samba \
    && apt-get purge -y --auto-remove $BUILD_DEP \
    && rm -rf /var/lib/apt/lists/*

ENV PATH /usr/local/samba/bin/:/usr/local/samba/sbin/:$PATH

EXPOSE 137/udp 138/udp 139 445

RUN mkdir /home/share \
    && chmod 777 /home/share

COPY smb.conf /usr/local/samba/etc/smb.conf

CMD ["/usr/local/samba/sbin/smbd", "-FS"]