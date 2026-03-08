FROM grafana/grafana:11.0.0-ubuntu

LABEL maintainer="phithon <root@leavesongs.com>"

USER root

RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends wget unzip \
    && wget -q -O /tmp/duckdb.zip https://github.com/duckdb/duckdb/releases/download/v1.0.0/duckdb_cli-linux-amd64.zip \
    && unzip /tmp/duckdb.zip -d /usr/local/bin/ \
    && chmod +x /usr/local/bin/duckdb \
    && rm /tmp/duckdb.zip \
    && apt-get purge -y wget unzip \
    && apt-get autoremove -y \
    && rm -rf /var/lib/apt/lists/* \
    && mkdir -p /usr/share/grafana/.duckdb \
    && chown grafana:root /usr/share/grafana/.duckdb

USER grafana
