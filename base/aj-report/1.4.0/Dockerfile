FROM eclipse-temurin:8u412-b08-jre-jammy

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends unzip wait-for-it \
    && mkdir -p /opt/aj-report \
    && wget -O /tmp/aj-report-1.4.0.zip https://gitee.com/anji-plus/report/releases/download/V1.4.0/aj-report-1.4.0.zip \
    && unzip /tmp/aj-report-1.4.0.zip -d /opt/aj-report \
    && rm -rf /tmp/aj-report-1.4.0.zip

WORKDIR /opt/aj-report/aj-report-1.4.0.RELEASE
COPY bootstrap.yml /opt/aj-report/aj-report-1.4.0.RELEASE/conf/bootstrap.yml
COPY docker-entrypoint.sh /docker-entrypoint.sh

ENTRYPOINT [ "bash", "/docker-entrypoint.sh" ]
CMD [ "java", "-agentlib:jdwp=transport=dt_socket,server=y,suspend=n,address=5005", "-Xbootclasspath/a:lib/*.jar", "-Dspring.config.location=conf/bootstrap.yml", "-jar", "lib/aj-report-1.4.0.RELEASE.jar" ]
