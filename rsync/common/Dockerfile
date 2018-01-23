FROM vulhub/rsync:3.1.2

MAINTAINER phithon <root@leavesongs.com>

ADD rsyncd.conf /etc/rsyncd.conf

ADD docker-entrypoint.sh /docker-entrypoint.sh

RUN apt-get update \
    && apt-get install --no-install-recommends -y cron \
    && mkdir /data/ \
    && chmod +x /docker-entrypoint.sh \
    && rm -rf /var/lib/apt/lists/*

CMD ["/docker-entrypoint.sh"]
