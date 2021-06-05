FROM debian:jessie

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && apt-get -o Acquire::GzipIndexes=false update \
    && apt-get install -y --no-install-recommends apt-show-versions gdebi-core wget ca-certificates \
    && wget -O /tmp/webmin.deb https://sourceforge.net/projects/webadmin/files/webmin/1.910/webmin_1.910_all.deb/download \
    && gdebi -n /tmp/webmin.deb \
    && apt-get purge -y --auto-remove gdebi-core \
    && rm -rf /var/lib/apt/lists/* /tmp/webmin.deb

COPY docker-entrypoint.sh /
EXPOSE 10000

RUN set -ex \
    && sed -i 's/passwd_mode=.*/passwd_mode=2/' /etc/webmin/miniserv.conf \
    && chmod +x /docker-entrypoint.sh \
    && echo 'root:vulhub' | chpasswd

ENTRYPOINT ["/docker-entrypoint.sh"]
