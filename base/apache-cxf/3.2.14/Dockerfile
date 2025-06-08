FROM maven:3.9.9-eclipse-temurin-8 AS builder

LABEL maintainer="ReaJason <reajason1225@gmail.com>"

COPY ./ /usr/src

RUN set -ex \
    && cd /usr/src \
    && mvn clean package -DskipTests

FROM openjdk:8u342-jre

LABEL maintainer="ReaJason <reajason1225@gmail.com>"

WORKDIR /app

COPY --from=builder /usr/src/target/*.jar /app/cxf.jar

EXPOSE 8080

CMD java $JAVA_OPTS -jar /app/cxf.jar