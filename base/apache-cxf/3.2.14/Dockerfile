FROM maven:3.9.9-eclipse-temurin-8 AS builder

LABEL maintainer="ReaJason <reajason1225@gmail.com>"

WORKDIR /usr/src

RUN set -ex \
    && git clone https://github.com/ReaJason/CVE-2024-28752.git \
    && cd CVE-2024-28752 \
    && mvn clean package -DskipTests

FROM openjdk:8u342-jre

LABEL maintainer="ReaJason <reajason1225@gmail.com>"

WORKDIR /app

COPY --from=builder /usr/src/CVE-2024-28752/target/*.jar /app/cxf.jar

EXPOSE 8080

CMD java $JAVA_OPTS -jar /app/cxf.jar