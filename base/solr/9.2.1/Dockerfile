FROM openjdk:11.0.16-jre

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && sed -i 's|deb http://deb.debian.org|# deb http://deb.debian.org|g' /etc/apt/sources.list \
    && sed -i 's|# deb http://snapshot.debian.org|deb [trusted=yes] http://snapshot.debian.org|g' /etc/apt/sources.list \
    && apt-get -o Acquire::Check-Valid-Until=false update \
    && apt-get install -y --no-install-recommends procps \
    && rm -rf /var/lib/apt/lists/*

RUN set -ex \
    && mkdir -p /opt/solr \
    && wget --no-check-certificate -qO- https://archive.apache.org/dist/solr/solr/9.2.1/solr-9.2.1.tgz | tar xz --strip-components=1 -C /opt/solr

COPY docker-entrypoint.sh /docker-entrypoint.sh
ENV PATH="$PATH:/opt/solr/bin" SOLR_JETTY_HOST="0.0.0.0"
WORKDIR /opt/solr
ENTRYPOINT [ "bash", "/docker-entrypoint.sh" ]
CMD ["solr", "-f", "-force", "-p", "8983", "-a", "-agentlib:jdwp=transport=dt_socket,server=y,suspend=n,address=*:5005"]
