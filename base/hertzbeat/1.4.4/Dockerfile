FROM node:18-slim AS frontend-builder

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends wget ca-certificates \
    && wget -qO- https://github.com/apache/hertzbeat/archive/refs/tags/v1.4.4.tar.gz | tar xz -C /usr/src --strip-components=1 \
    && cd /usr/src/web-app \
    && yarn \
    && yarn build \
    && rm -rf /var/lib/apt/lists/*

FROM maven:3.9.9-eclipse-temurin-11 AS backend-builder

LABEL maintainer="phithon <root@leavesongs.com>"

COPY --from=frontend-builder /usr/src /usr/src

RUN set -ex \
    && cd /usr/src \
    && mvn clean install -DskipTests

FROM eclipse-temurin:11-jdk

LABEL maintainer="phithon <root@leavesongs.com>"

COPY --from=backend-builder /usr/src/manager/target/hertzbeat-1.4.4.tar.gz /hertzbeat-1.4.4.tar.gz

RUN set -ex \
    && tar xzf /hertzbeat-1.4.4.tar.gz -C /opt \
    && rm -rf /hertzbeat-1.4.4.tar.gz

WORKDIR /opt/hertzbeat

EXPOSE 1157

CMD ["java", "-Doracle.jdbc.timezoneAsRegion=false", "-Dlogging.path=logs", "-Dlogging.config=config/logback-spring.xml", "-Dspring.config.location=config/", "-agentlib:jdwp=transport=dt_socket,server=y,suspend=n,address=*:5005", "-jar", "hertzbeat.jar", "--spring.profiles.active=prod"]
