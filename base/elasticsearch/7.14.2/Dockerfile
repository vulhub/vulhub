FROM openjdk:8u102-jre

LABEL maintainer="phithon <root@leavesongs.com>"

ARG ESHOME=/usr/share/elasticsearch
COPY docker-entrypoint.sh /

RUN set -ex \
    && mkdir -p ${ESHOME} \
    && wget -qO- https://artifacts.elastic.co/downloads/elasticsearch/elasticsearch-7.16.0-no-jdk-linux-x86_64.tar.gz | tar zx \
        --strip-components 1 -C ${ESHOME} \
    && chmod +x /docker-entrypoint.sh

ENV PATH ${ESHOME}/bin:$PATH
WORKDIR ${ESHOME}
EXPOSE 9200 9300

COPY elasticsearch.yml ./config/
RUN sed -ex \
    && chown nobody:nogroup -R ${ESHOME}

USER nobody:nogroup
ENTRYPOINT ["/docker-entrypoint.sh"]
CMD ["elasticsearch"]
