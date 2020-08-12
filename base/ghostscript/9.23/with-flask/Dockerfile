FROM vulhub/ghostscript:9.23

LABEL maintainer="phith0n <root@leavesongs.com>"

RUN set -ex \
    && apt-get update \
    && apt-get install --no-install-recommends -y python3 python3-pip \
    && update-alternatives --install /usr/bin/python python /usr/bin/python3 100 \
    && rm -rf /var/lib/apt/lists/*

RUN set -ex \
    && python3 -m pip install -U pip \
    && python3 -m pip install "flask==1.0.2" "Pillow==5.3.0"

WORKDIR /usr/src
