FROM vulhub/imagemagick:7.1.0-49

LABEL maintainer="phith0n <root@leavesongs.com>"

RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends php-cli \
    && rm -rf /var/lib/apt/lists/*
