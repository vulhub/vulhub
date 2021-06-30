FROM maven:3.8.1-jdk-8 AS builder

LABEL maintainer="phithon <root@leavesongs.com>"

COPY ./ /usr/src/

RUN set -ex \
    && cd /usr/src \
    && mvn clean package

FROM openjdk:8-jre

COPY --from=builder /usr/src/target/dubbo-sample-1.0-SNAPSHOT.jar /dubbo-sample-1.0-SNAPSHOT.jar
COPY docker-entrypoint.sh /docker-entrypoint.sh

EXPOSE 8080

CMD ["bash", "/docker-entrypoint.sh"]
