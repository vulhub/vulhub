FROM maven:3.8-jdk-8 AS builder

LABEL maintainer="phithon <root@leavesongs.com>"

COPY ./ /usr/src/

RUN set -ex \
    && cd /usr/src \
    && mvn clean package

FROM eclipse-temurin:8-jre-jammy

COPY --from=builder /usr/src/target/dubbo-sample-1.0-SNAPSHOT.jar /dubbo-sample-1.0-SNAPSHOT.jar
COPY docker-entrypoint.sh /docker-entrypoint.sh

WORKDIR /

EXPOSE 20880 5005

ENTRYPOINT ["bash", "/docker-entrypoint.sh"]
CMD ["consumer"]
