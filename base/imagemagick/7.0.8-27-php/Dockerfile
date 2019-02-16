FROM vulhub/imagemagick:7.0.8-27-php

LABEL maintainer="phith0n <root@leavesongs.com>"

RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends php5-cli \
    && rm -rf /var/lib/apt/lists/*