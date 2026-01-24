FROM maven:3-jdk-8 AS builder

LABEL maintainer="lanyi"

COPY ./ /usr/src/
WORKDIR /usr/src

RUN set -ex \
    && mvn package

FROM openjdk:8u102-jre

COPY --from=builder /usr/src/target/postgreRce-0.0.1-SNAPSHOT.jar /usr/src/postgreRce.jar
EXPOSE 8090

CMD [ "java", "-Dserver.address=0.0.0.0", "-Dserver.port=8090", "-jar", "/usr/src/postgreRce.jar" ]