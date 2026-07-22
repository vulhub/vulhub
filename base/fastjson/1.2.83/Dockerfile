FROM maven:3-eclipse-temurin-8 AS builder

LABEL maintainer="phithon <root@leavesongs.com>"

COPY ./ /usr/src/
WORKDIR /usr/src

RUN set -ex \
    && mvn package

FROM eclipse-temurin:8-jre

COPY --from=builder /usr/src/target/fastjsondemo-1.0.0-SNAPSHOT.jar /usr/src/fastjsondemo.jar
EXPOSE 8090 5005

CMD [ "java", "-agentlib:jdwp=transport=dt_socket,server=y,suspend=n,address=5005", "-Dserver.address=0.0.0.0", "-Dserver.port=8090", "-jar", "/usr/src/fastjsondemo.jar" ]
