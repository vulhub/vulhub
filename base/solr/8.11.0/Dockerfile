FROM openjdk:8u102-jre

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && mkdir -p /opt/solr \
    && wget -qO- https://archive.apache.org/dist/lucene/solr/8.11.0/solr-8.11.0.tgz | tar xz --strip-components=1 -C /opt/solr

WORKDIR /opt/solr
CMD ["/opt/solr/bin/solr", "-f", "-force", "-p", "8983"]
