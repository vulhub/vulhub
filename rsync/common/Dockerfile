FROM vulhub/rsync:3.1.2

MAINTAINER phithon <root@leavesongs.com>

ADD rsyncd.conf /etc/rsyncd.conf

ADD docker-entrypoint.sh /docker-entrypoint.sh

RUN sed -i 's/deb.debian.org/mirrors.ustc.edu.cn/g' /etc/apt/sources.list \
    && sed -i 's|security.debian.org|mirrors.ustc.edu.cn/debian-security/|g' /etc/apt/sources.list \
    && apt-get update \
    && apt-get install --no-install-recommends -y cron \
    && mkdir /data/ \
    && chmod +x /docker-entrypoint.sh \
    && rm -rf /var/lib/apt/lists/*

CMD ["/docker-entrypoint.sh"]
