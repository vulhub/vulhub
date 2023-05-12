FROM vulhub/rsync:3.1.2

LABEL maintainer="phithon <root@leavesongs.com>"

COPY rsyncd.conf /etc/rsyncd.conf

COPY docker-entrypoint.sh /docker-entrypoint.sh

RUN apt-get update \
    && apt-get install --no-install-recommends -y cron \
    && mkdir /data/ \
    && chmod +x /docker-entrypoint.sh \
    && rm -rf /var/lib/apt/lists/*

CMD ["/docker-entrypoint.sh"]
