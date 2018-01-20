FROM vulhub/cron

MAINTAINER phithon <root@leavesongs.com>

RUN set -ex \
    && yum -y update \
    && yum -y install gcc-c++ tcl wget

RUN set -ex \
    && mkdir -p /usr/src/redis \
    && wget -qO- http://download.redis.io/releases/redis-2.8.24.tar.gz | tar xz -C /usr/src/redis --strip-components=1 \
    && cd /usr/src/redis \
    && make MALLOC=libc \
    && make install \
    && make clean \
    && cd / \
    && rm -rf /usr/src/redis

RUN yum -y remove gcc-c++ tcl

COPY docker-entrypoint.sh /usr/local/bin/

RUN set -ex \
    && chmod +x /usr/local/bin/docker-entrypoint.sh

EXPOSE 6379

ENTRYPOINT ["/usr/local/bin/docker-entrypoint.sh"]
CMD ["redis-server"]
