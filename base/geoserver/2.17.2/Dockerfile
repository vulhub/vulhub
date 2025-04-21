FROM openjdk:17.0.2-slim

LABEL maintainer="phithon <root@leavesongs.com>"

ARG GEOSERVER_VERSION=2.17.2
RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends curl unzip ca-certificates \
    && curl -#SL https://sourceforge.net/projects/geoserver/files/GeoServer/${GEOSERVER_VERSION}/geoserver-${GEOSERVER_VERSION}-bin.zip/download -o /tmp/geoserver.zip \
    && curl -#SL https://sourceforge.net/projects/geoserver/files/GeoServer/${GEOSERVER_VERSION}/extensions/geoserver-${GEOSERVER_VERSION}-wps-plugin.zip/download -o /tmp/geoserver-wps-plugin.zip \
    && mkdir /mnt/geoserver \
    && unzip /tmp/geoserver.zip -d /mnt/geoserver \
    && unzip -o /tmp/geoserver-wps-plugin.zip -d /mnt/geoserver/webapps/geoserver/WEB-INF/lib -x "*.md" \
    && rm -rf /tmp/geoserver.zip /tmp/geoserver-wps-plugin.zip

EXPOSE 8080
WORKDIR /mnt/geoserver

ENV JAVA_OPTS="-agentlib:jdwp=transport=dt_socket,server=y,suspend=n,address=*:5005"
CMD [ "/mnt/geoserver/bin/startup.sh" ]
