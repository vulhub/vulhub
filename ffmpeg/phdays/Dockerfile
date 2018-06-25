FROM vulhub/ffmpeg:3.2.4

MAINTAINER phithon <root@leavesongs.com>

RUN set -ex \
    && sed -i 's/deb.debian.org/mirrors.ustc.edu.cn/g' /etc/apt/sources.list \
    && apt-get update \
    && apt-get install -y --no-install-recommends php-cli

COPY ./www /usr/local/src

WORKDIR /usr/local/src