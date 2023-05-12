FROM vulhub/ghostscript:9.23

LABEL maintainer="phith0n <root@leavesongs.com>"

RUN set -ex \
    && apt-get update \
    && apt-get install --no-install-recommends -y python3 python3-pip python3-setuptools \
    && update-alternatives --install /usr/bin/python python /usr/bin/python3 100 \
    && rm -rf /var/lib/apt/lists/*

RUN set -ex \
    && python3 -m pip install "pip==19.3.1" \
    && python3 -m pip install "flask==1.0.2" "Pillow==5.3.0"

WORKDIR /usr/src
