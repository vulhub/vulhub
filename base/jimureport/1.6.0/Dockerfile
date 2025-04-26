FROM maven:3-eclipse-temurin-17 AS builder

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && wget -qO- https://github.com/jeecgboot/JimuReport/archive/refs/tags/v1.6.0.tar.gz | tar xz --strip-components=1 -C /usr/src \
    && cd /usr/src/jimureport-example \
    && sed -i 's|MYSQL-HOST|MYSQL_HOST|i' src/main/resources/application.yml \
    && sed -i 's|MYSQL-PORT|MYSQL_PORT|i' src/main/resources/application.yml \
    && sed -i 's|MYSQL-DB|MYSQL_DB|i' src/main/resources/application.yml \
    && sed -i 's|username: root|username: \${MYSQL_USERNAME:root}|i' src/main/resources/application.yml \
    && sed -i 's|password: root|password: \${MYSQL_PASSWORD:root}|i' src/main/resources/application.yml \
    && sed -i 's|http://maven.aliyun.com|https://maven.aliyun.com|i' pom.xml \
    && sed -i 's|http://maven.jeecg.org|https://maven.jeecg.org|i' pom.xml \
    && mvn package -DskipTests

FROM eclipse-temurin:17-jdk-jammy

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && mkdir /opt/jimureport /opt/upload \
    && apt-get update \
    && apt-get install -y --no-install-recommends mysql-client

COPY --from=builder /usr/src/jimureport-example/target/jimureport-example-1.6.jar /opt/jimureport/jimureport-example-1.6.jar
COPY --from=builder /usr/src/db/jimureport.mysql5.7.create.sql /opt/jimureport/jimureport.mysql5.7.create.sql
COPY docker-entrypoint.sh /docker-entrypoint.sh

WORKDIR /opt/jimureport
ENTRYPOINT [ "bash", "/docker-entrypoint.sh" ]
CMD [ "java", "-agentlib:jdwp=transport=dt_socket,server=y,suspend=n,address=*:5005", "-jar", "/opt/jimureport/jimureport-example-1.6.jar" ]
