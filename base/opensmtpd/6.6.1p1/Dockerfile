FROM ubuntu:focal

LABEL maintainer="ph1sh && phithon <root@leavesongs.com>"

RUN set -ex \
    && apt-get update \
    && apt-get install --no-install-recommends -y ca-certificates wget bison autoconf automake gcc make libc-dev libasr-dev libssl-dev libevent-dev zlib1g-dev \
    && cd /usr/src \
    && wget -qO- https://www.opensmtpd.org/archives/opensmtpd-6.6.1p1.tar.gz | tar xz --strip-components=1 \
    && ./configure \
    && make \
    && make install \
    && cd / \
    && apt-get purge -y --auto-remove bison autoconf automake gcc make \
    && rm -rf /var/lib/apt/lists/* /usr/src/*

RUN set -ex \
    && mkdir /var/empty \
    && useradd -c "SMTP Daemon" -d /var/empty -s /sbin/nologin _smtpd \
    && useradd -c "SMTPD Queue" -d /var/empty -s /sbin/nologin _smtpq

COPY aliases /etc/
COPY smtpd.conf /usr/local/etc/

WORKDIR /tmp
EXPOSE 25
CMD ["smtpd", "-dv"]
