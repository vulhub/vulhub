FROM neo4j:3.4.18

LABEL maintainer="phithon <root@leavesongs.com>"

# allow remote shell server for all interfaces
RUN set -ex \
    && sed -i "s/#dbms.shell.enabled=true/dbms.shell.enabled=true/g" /var/lib/neo4j/conf/neo4j.conf \
    && sed -i "s/#dbms.shell.host=127.0.0.1/dbms.shell.host=0.0.0.0/g" /var/lib/neo4j/conf/neo4j.conf
