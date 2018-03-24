FROM maven:3-jdk-8 AS builder

LABEL maintainer="phithon <root@leavesongs.com>"

COPY src/ /usr/src/

WORKDIR /usr/src

RUN cd /usr/src; \
    mvn clean install

# production
FROM openjdk:8-jre

COPY --from=builder /usr/src/target/demo-0.0.1-SNAPSHOT.jar /spring-security-oauth2-0.0.1-SNAPSHOT.jar

CMD ["java", "-Djava.security.egd=file:/dev/./urandom", "-jar", "/spring-security-oauth2-0.0.1-SNAPSHOT.jar"]