FROM vulhub/openssh:7.7

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && adduser --home /home/vulhub --shell /bin/bash --disabled-password --gecos "" vulhub \
    && echo "vulhub:vulhub" | chpasswd \
    && adduser --home /home/example --shell /bin/bash --disabled-password --gecos "" example \
    && echo "example:123456" | chpasswd