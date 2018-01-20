FROM debian:jessie

MAINTAINER phithon <root@leavesongs.com>

ADD https://github.com/vishnubob/wait-for-it/raw/master/wait-for-it.sh /wait-for-it.sh

RUN set -ex \
    && apt-get update \
    && apt-get install curl -y --no-install-recommends \
    && chmod +x /wait-for-it.sh \
    && { \
        echo '#!/bin/bash'; \
        echo '/wait-for-it.sh ${COUCHDB_URL}'; \
        echo 'curl -X PUT http://${COUCHDB_AUTH}@${COUCHDB_URL}/_users'; \
        echo 'curl -X PUT http://${COUCHDB_AUTH}@${COUCHDB_URL}/_replicator'; \
        echo 'curl -X PUT http://${COUCHDB_AUTH}@${COUCHDB_URL}/_global_changes'; \
    } | tee /run.sh \
    && chmod +x /run.sh \
    && rm -rf /var/lib/apt/lists/*

CMD ["/run.sh"]