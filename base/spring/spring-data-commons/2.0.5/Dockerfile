FROM maven:3-jdk-8 AS builder

COPY ./ /usr/src

RUN cd /usr/src; \
    mvn clean install

FROM openjdk:8-jre

LABEL maintainer="phithon <root@leavesongs.com>"

COPY --from=builder /usr/src/target/spring-data-web-example-2.0.0.RELEASE.jar /spring-data-web-example-2.0.0.RELEASE.jar

CMD ["java", "-Djava.security.egd=file:/dev/./urandom", "-jar", "/spring-data-web-example-2.0.0.RELEASE.jar"]