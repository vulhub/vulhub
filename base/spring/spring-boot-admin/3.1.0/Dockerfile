FROM maven:3.9-eclipse-temurin-17 AS builder

LABEL maintainer="phithon <root@leavesongs.com>"

WORKDIR /usr/src

COPY pom.xml /usr/src/pom.xml
COPY src /usr/src/src

RUN mvn -U clean package -Dmaven.test.skip=true

FROM eclipse-temurin:17-jre

LABEL maintainer="phithon <root@leavesongs.com>"

COPY --from=builder /usr/src/target/app.jar /app.jar

EXPOSE 8080 5005

CMD ["java", "-agentlib:jdwp=transport=dt_socket,server=y,suspend=n,address=*:5005", "-jar", "/app.jar"]
