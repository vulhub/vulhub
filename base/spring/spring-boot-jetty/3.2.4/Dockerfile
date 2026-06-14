FROM maven:3.9-eclipse-temurin-17 AS builder

COPY ./ /usr/src

RUN cd /usr/src; \
    mvn -U clean package -Dmaven.test.skip=true -Dmaven.artifact.threads=10

FROM eclipse-temurin:17-jre

LABEL maintainer="phithon <root@leavesongs.com>"

COPY --from=builder /usr/src/target/app.jar /app.jar

EXPOSE 8080 5005

CMD ["java", "-Djava.security.egd=file:/dev/./urandom", "-agentlib:jdwp=transport=dt_socket,server=y,suspend=n,address=*:5005", "-jar", "/app.jar"]
