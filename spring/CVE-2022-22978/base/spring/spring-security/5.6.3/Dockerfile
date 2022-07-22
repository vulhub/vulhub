FROM maven:3-jdk-8 AS builder
COPY ./code/ /usr/src/
WORKDIR /usr/src
RUN cd /usr/src; \
    mvn -U clean package -Dmaven.test.skip=true


FROM openjdk:8-jdk-alpine
COPY --from=builder /usr/src/target/com.vulhub.authzvuln-0.0.1-SNAPSHOT.jar /demo.jar

EXPOSE 8080

CMD ["java", "-jar", "/demo.jar"]

