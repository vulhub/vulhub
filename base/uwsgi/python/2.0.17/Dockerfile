FROM buildpack-deps:stretch-curl

LABEL maintainer="phithon <root@leavesongs.com>"

ARG DEBIAN_MIRROR_URL=https://github.com/vulhub/mirrors/raw/master/debian/stretch/aliyun.list
ARG PIP_MIRROR_URL=https://pypi.tuna.tsinghua.edu.cn/simple

RUN set -ex \
    && wget $DEBIAN_MIRROR_URL -O /etc/apt/sources.list \
    && apt-get update \
    && apt-get install --no-install-recommends -y build-essential python3 python3-dev python3-pip \
    && pip3 install -i $PIP_MIRROR_URL -U pip setuptools \
    && pip install -i $PIP_MIRROR_URL "uwsgi==2.0.17" \
    && apt-get purge --autoremove -y build-essential \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /usr/src
