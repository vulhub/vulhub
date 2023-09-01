FROM vulhub/rsync:3.1.2

LABEL maintainer="phithon <root@leavesongs.com>"

COPY rsyncd.conf /etc/rsyncd.conf

COPY docker-entrypoint.sh /docker-entrypoint.sh

RUN echo "deb http://snapshot.debian.org/archive/debian/20210326T030000Z jessie main" > /etc/apt/sources.list

RUN apt-get update \
    && apt-get install --no-install-recommends -y cron --force-yes \
    && mkdir /data/ \
    && chmod +x /docker-entrypoint.sh \
    && rm -rf /var/lib/apt/lists/*

CMD ["/docker-entrypoint.sh"]
