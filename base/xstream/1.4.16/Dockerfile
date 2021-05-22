FROM maven:3.8.1-jdk-8 AS builder

COPY ./ /usr/src/

RUN set -ex \
    && cd /usr/src \
    && mvn package

FROM vulhub/java:8u102-jdk

LABEL maintainer="phithon <root@leavesongs.com>"

COPY --from=builder /usr/src/target/xstream-sample-0.0.1-SNAPSHOT.jar /xstream-sample-0.0.1-SNAPSHOT.jar

EXPOSE 8080
CMD ["java", "-jar", "/xstream-sample-0.0.1-SNAPSHOT.jar"]
