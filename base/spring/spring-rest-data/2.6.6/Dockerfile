FROM maven:3-jdk-8 AS builder

LABEL maintainer="phithon <root@leavesongs.com>"

COPY ./ /usr/src/

WORKDIR /usr/src

RUN cd /usr/src; \
    mvn clean install

# production
FROM openjdk:8-jre

LABEL maintainer="phithon <root@leavesongs.com>"

COPY --from=builder /usr/src/target/spring-rest-data-demo-2.0.0.BUILD.jar /spring-rest-data-demo-2.0.0.BUILD.jar

CMD ["java", "-Djava.security.egd=file:/dev/./urandom", "-jar", "/spring-rest-data-demo-2.0.0.BUILD.jar"]