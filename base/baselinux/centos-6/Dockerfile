FROM centos:6

LABEL maintainer="phithon <root@leavesongs.com>"

COPY ./repo.list /etc/yum.repos.d/CentOS-Base.repo

RUN set -ex \
    && yum -y install wget cronie gcc-c++ tcl \
    && mkdir -p /usr/src/redis \
    && wget -qO- http://download.redis.io/releases/redis-2.8.24.tar.gz | tar xz -C /usr/src/redis --strip-components=1 \
    && cd /usr/src/redis \
    && make MALLOC=libc \
    && make install \
    && make clean \
    && cd / \
    && rm -rf /usr/src/redis \
    && yum -y remove gcc-c++ tcl

COPY docker-entrypoint.sh /
RUN set -ex \
    && chmod +x /docker-entrypoint.sh

EXPOSE 6379

ENTRYPOINT ["/docker-entrypoint.sh"]
CMD ["redis-server"]
