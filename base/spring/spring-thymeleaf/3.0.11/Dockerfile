FROM maven:3.8-eclipse-temurin-8 AS builder

LABEL maintainer="phithon <root@leavesongs.com>"

WORKDIR /usr/src

COPY pom.xml /usr/src/pom.xml
COPY src /usr/src/src

RUN mvn -U clean package -Dmaven.test.skip=true

FROM eclipse-temurin:8-jre

LABEL maintainer="phithon <root@leavesongs.com>"

COPY --from=builder /usr/src/target/app.jar /app.jar

EXPOSE 8090 5005

ENV JAVA_OPTS=""

CMD ["sh", "-c", "java $JAVA_OPTS -agentlib:jdwp=transport=dt_socket,server=y,suspend=n,address=5005 -jar /app.jar"]
