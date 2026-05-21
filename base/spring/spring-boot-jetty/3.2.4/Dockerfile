FROM maven:3.9-eclipse-temurin-17 AS builder

WORKDIR /usr/src

COPY pom.xml .
RUN mvn -U dependency:resolve dependency:resolve-plugins

COPY src ./src
RUN mvn package -DskipTests

FROM eclipse-temurin:17-jre

LABEL maintainer="phithon <root@leavesongs.com>"

WORKDIR /app

COPY --from=builder /usr/src/target/*.jar app.jar

EXPOSE 8080 5005

ENV JAVA_OPTS=""

ENTRYPOINT ["sh", "-c", "java $JAVA_OPTS -agentlib:jdwp=transport=dt_socket,server=y,suspend=n,address=*:5005 -jar app.jar"]