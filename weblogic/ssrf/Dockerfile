FROM centos:6

MAINTAINER phithon <root@leavesongs.com>

RUN set -ex \
    && yum update \
    && yum install -y gcc-c++ tcl wget cronie

RUN set -ex \
    && mkdir -p /usr/bin/src \
    && wget -qO- http://download.redis.io/releases/redis-2.8.24.tar.gz | tar xz -C /usr/src/wordpress --strip-components=1 \
    && cd /usr/bin/src \
    && make MALLOC=libc \
    && make install \
    && make clean \
    && cd / \
    && rm -rf /usr/bin/src

RUN yum -y remove gcc-c++ tcl

COPY docker-entrypoint.sh /usr/local/bin/

RUN set -ex \
    && chmod +x /usr/local/bin/docker-entrypoint.sh

ENTRYPOINT /usr/local/bin/docker-entrypoint.sh
CMD ["redis-server"]
