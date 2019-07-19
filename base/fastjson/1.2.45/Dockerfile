FROM maven:3-jdk-8 AS builder

LABEL maintainer="phithon <root@leavesongs.com>"

COPY ./src pom.xml /usr/src/
WORKDIR /usr/src

RUN set -ex \
    && mvn package

FROM openjdk:8u222-jre

COPY --from=builder /usr/src/target/fastjsondemo-SNAPSHOT.jar /usr/src/fastjsondemo-SNAPSHOT.jar

CMD [ "java", "-Dserver.address=0.0.0.0", "-Dserver.port=8090", "-jar", "/usr/src/fastjsondemo-SNAPSHOT.jar" ]