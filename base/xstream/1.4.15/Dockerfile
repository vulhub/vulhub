FROM maven:3.8.1-jdk-8 AS builder

COPY ./ /usr/src/

RUN set -ex \
    && cd /usr/src \
    && mvn package

FROM openjdk:8-jre

LABEL maintainer="phithon <root@leavesongs.com>"

COPY --from=builder /usr/src/target/xstream-sample-1.4.15-SNAPSHOT.jar /xstream-sample.jar

EXPOSE 8080
CMD ["java", "-jar", "/xstream-sample.jar"]
