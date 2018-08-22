FROM vulhub/ghostscript:9.21

LABEL maintainer="phith0n <root@leavesongs.com>"

ADD https://bootstrap.pypa.io/get-pip.py /get-pip.py

RUN apt-get update \
    && apt-get install --no-install-recommends -y python3 \
    && python3 /get-pip.py \
    && update-alternatives --install /usr/bin/python python /usr/bin/python3 100 \
    && rm -rf /var/lib/apt/lists/*
