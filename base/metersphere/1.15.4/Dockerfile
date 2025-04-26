FROM node:16.20.1-buster AS frontend

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends wget \
    && wget -qO- https://github.com/metersphere/metersphere/archive/refs/tags/v1.15.4.tar.gz | tar xz --strip-components 1 -C /usr/src \
    && cd /usr/src/frontend \
    && npm install \
    && npm run build

FROM maven:3.9.3 AS backend

LABEL maintainer="phithon <root@leavesongs.com>"

COPY --from=frontend /usr/src /usr/src

RUN set -ex \
    && cd /usr/src/backend \
    && mvn clean package dependency:copy-dependencies

FROM openjdk:8u342-jdk

COPY docker-entrypoint.sh /docker-entrypoint.sh
RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends wait-for-it \
    && mkdir -p /opt/metersphere/conf /opt/jmeter/bin \
    && chmod +x /docker-entrypoint.sh \
    && rm -rf /var/lib/apt/lists/*

COPY --from=backend /usr/src/backend/target/classes/ /opt/metersphere/
COPY --from=backend /usr/src/backend/src/main/resources/jmeter/bin/* /opt/jmeter/bin/
COPY --from=backend /usr/src/backend/target/wars/ /opt/metersphere/lib/
COPY metersphere.properties /opt/metersphere/conf/metersphere.properties
WORKDIR /opt/metersphere

EXPOSE 5005 8081
ENTRYPOINT [ "bash", "/docker-entrypoint.sh" ]
CMD [ "java", "-agentlib:jdwp=transport=dt_socket,server=y,suspend=n,address=5005", "-cp", ".:./lib/*", "io.metersphere.Application" ]
