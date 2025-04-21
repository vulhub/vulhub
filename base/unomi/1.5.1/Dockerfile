FROM openjdk:8u272-jre

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && mkdir /usr/local/unomi \
    && cd /usr/local/unomi \
    && wget -qO- https://archive.apache.org/dist/unomi/1.5.1/unomi-1.5.1-bin.tar.gz | tar xz --strip-components=1

WORKDIR /usr/local/unomi
ENV KARAF_OPTS "-Dunomi.autoStart=true"
ENV UNOMI_ELASTICSEARCH_ADDRESSES "localhost:9200"
COPY docker-entrypoint.sh /

ENTRYPOINT ["/docker-entrypoint.sh"]
CMD ["bin/karaf", "server"]
