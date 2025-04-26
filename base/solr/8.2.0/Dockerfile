FROM openjdk:8u342-jdk

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && mkdir -p /opt/solr \
    && wget --no-check-certificate -qO- https://archive.apache.org/dist/lucene/solr/8.2.0/solr-8.2.0.tgz | tar xz --strip-components=1 -C /opt/solr

COPY docker-entrypoint.sh /docker-entrypoint.sh
ENV PATH="$PATH:/opt/solr/bin"
WORKDIR /opt/solr
ENTRYPOINT [ "bash", "/docker-entrypoint.sh" ]
CMD ["solr", "-f", "-force", "-p", "8983", "-a", "-agentlib:jdwp=transport=dt_socket,server=y,suspend=n,address=5005"]
