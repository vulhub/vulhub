FROM vulhub/redis:2.8

MAINTAINER phithon <root@leavesongs.com>

RUN set -ex \
    && apt-get update \
    && apt-get install -y cron --no-install-recommends \
    && rm -rf /var/lib/apt/lists/*

COPY docker-entrypoint.sh /usr/local/bin/

RUN set -ex \
    && chmod +x /usr/local/bin/docker-entrypoint.sh