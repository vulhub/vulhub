FROM vulhub/openssl:1.1.1m

LABEL maintainer="phithon <root@leavesongs.com>"

ADD https://github.com/curl/curl/releases/download/curl-7_82_0/curl-7.82.0.tar.gz /curl-7.82.0.tar.gz

RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends gcc make procps \
    && tar -zxvf /curl-7.82.0.tar.gz -C /usr/src --strip-components=1 \
    && cd /usr/src \
    && ./configure --with-openssl \
    && make \
    && make install \
    && ln -s /usr/local/lib/libcurl.so.4 /usr/lib/libcurl.so.4 \
    && apt-get purge -y --auto-remove gcc make \
    && rm -rf /usr/src/* /var/lib/apt/lists/*
