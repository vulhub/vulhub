FROM uozi/nginx-ui:v2.3.2

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends sqlite3 \
    && rm -rf /var/lib/apt/lists/* \
    && mkdir -p /etc/nginx-ui

COPY run /etc/s6-overlay/s6-rc.d/nginx-ui/run
