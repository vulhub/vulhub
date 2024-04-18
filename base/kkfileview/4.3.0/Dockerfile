FROM maven:3.9-eclipse-temurin-8 as builder

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && cd /usr/src \
    && wget -qO- https://github.com/kekingcn/kkFileView/archive/refs/tags/v4.3.0.tar.gz | tar zx --strip-components=1 \
    && mvn package -DskipTests

FROM openjdk:21-slim-bookworm

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends libreoffice \
    && mkdir /opt/kkfileview

COPY --from=builder /usr/src/server/target/kkFileView-4.3.0.jar /opt/kkfileview/kkFileView-4.3.0.jar
COPY --from=builder /usr/src/server/src/main/config/application.properties /opt/kkfileview/application.properties
WORKDIR /opt/kkfileview
CMD [ "java", "-Dfile.encoding=UTF-8", "-Dspring.configlocation=/opt/kkfileview/application.properties", "-jar", "kkFileView-4.3.0.jar" ]
