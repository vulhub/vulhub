FROM maven:3-jdk-8 AS builder

LABEL maintainer="phithon <root@leavesongs.com>"

COPY ./ /usr/src/
WORKDIR /usr/src

RUN set -ex \
    && mvn package

FROM openjdk:8u102-jre

COPY --from=builder /usr/src/target/fastjsondemo-1.2.24-SNAPSHOT.jar /usr/src/fastjsondemo.jar
EXPOSE 8090

CMD [ "java", "-Dserver.address=0.0.0.0", "-Dserver.port=8090", "-jar", "/usr/src/fastjsondemo.jar" ]