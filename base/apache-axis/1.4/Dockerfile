FROM buildpack-deps:curl AS download

RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends unzip \
    && wget -q "https://archive.apache.org/dist/ws/axis/1_4/axis-bin-1_4.zip" -O /tmp/axis.zip \
    && unzip -q /tmp/axis.zip -d /tmp/axis_extracted \
    && find /tmp/axis_extracted -path "*/webapps/axis" -type d \
       -exec cp -r {} /opt/axis \; \
    && rm -rf /tmp/axis.zip /tmp/axis_extracted

FROM vulhub/tomcat:9.0.30

LABEL maintainer="phithon <root@leavesongs.com>"

COPY --from=download /opt/axis /usr/local/tomcat/webapps/axis
COPY server-config.wsdd /usr/local/tomcat/webapps/axis/WEB-INF/server-config.wsdd

ENV JAVA_OPTS="-agentlib:jdwp=transport=dt_socket,server=y,address=5005,suspend=n"
