FROM maven:3-jdk-8 as builder

RUN set -ex \
    && wget -qO- https://github.com/neo4j/neo4j/archive/refs/tags/3.4.18.tar.gz | tar --strip-components 1 -xz -C /usr/src \
    && cd /usr/src/community/shell \
    && rm -rf src/test/

COPY pom.xml /usr/src/community/shell/pom.xml

RUN set -ex \
    && cd /usr/src/community/shell \
    && sed -i 's/super();/super(34444);/g' src/main/java/org/neo4j/shell/impl/RemotelyAvailableServer.java \
    && mvn clean package -DskipTests

FROM neo4j:3.4.18

LABEL maintainer="phithon <root@leavesongs.com>"

# allow remote shell server for all interfaces
RUN set -ex \
    && sed -i "s/#dbms.shell.enabled=true/dbms.shell.enabled=true/g" /var/lib/neo4j/conf/neo4j.conf \
    && sed -i "s/#dbms.shell.host=127.0.0.1/dbms.shell.host=0.0.0.0/g" /var/lib/neo4j/conf/neo4j.conf \
    && sed -i 's/JAVA_OPTS=("-server")/JAVA_OPTS+=("-server")/g' /var/lib/neo4j/bin/neo4j

COPY --from=builder /usr/src/community/shell/target/neo4j-shell-3.4.18.jar /var/lib/neo4j/lib/neo4j-shell-3.4.18.jar
