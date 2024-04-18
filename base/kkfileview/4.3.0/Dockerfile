FROM maven:3.9-eclipse-temurin-8 as builder

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && cd /usr/src \
    && wget -qO- https://github.com/kekingcn/kkFileView/archive/refs/tags/v4.3.0.tar.gz | tar zx --strip-components=1 \
    && mvn package -DskipTests

FROM ubuntu:20.04

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && export DEBIAN_FRONTEND=noninteractive \
    && apt-get update \
    && apt-get install -y --no-install-recommends wget ca-certificates libxrender1 libxinerama1 libxt6 libxext-dev libfreetype6-dev libcairo2 libcups2 libx11-xcb1 libnss3 libglib2.0-dev openjdk-8-jre \
    && mkdir /opt/kkfileview

RUN set -ex \
    && mkdir -p /tmp/libreoffice \
    && cd /tmp/libreoffice \
    && wget -qO- https://downloadarchive.documentfoundation.org/libreoffice/old/7.5.3.2/deb/x86_64/LibreOffice_7.5.3.2_Linux_x86-64_deb.tar.gz | tar xz --strip-components=1 \
    # && wget -qO- https://file.vulhub.org/download/libreoffice/LibreOffice_7.5.3.2_Linux_x86-64_deb.tar.gz | tar xz --strip-components=1 \
    && cd DEBS \
    && dpkg -i *.deb \
    && cd / \
    && rm -rf /tmp/libreoffice

COPY --from=builder /usr/src/server/target/kkFileView-4.3.0.tar.gz /opt/kkfileview/kkFileView-4.3.0.tar.gz

RUN set -ex \
    && cd /opt/kkfileview \
    && tar --strip-components=1 -zxvf kkFileView-4.3.0.tar.gz \
    && rm -rf kkFileView-4.3.0.tar.gz

ENV KKFILEVIEW_BIN_FOLDER /opt/kkfileview/bin
WORKDIR /opt/kkfileview/bin
CMD [ "java", "-Dfile.encoding=UTF-8", "-Dspring.config.location=/opt/kkfileview/config/application.properties", "-agentlib:jdwp=transport=dt_socket,server=y,suspend=n,address=5005", "-jar", "kkFileView-4.3.0.jar" ]
