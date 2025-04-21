FROM eclipse-temurin:8u412-b08-jre

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    # && apt-get update \
    && mkdir /opt/nacos \
    && cd /opt/nacos \
    && wget -qO- https://github.com/alibaba/nacos/releases/download/1.4.0/nacos-server-1.4.0.tar.gz | tar xz -C /opt/nacos --strip-components=1 \
    && sed -i 's|echo "\$JAVA \${JAVA_OPT}".*|echo "$JAVA ${JAVA_OPT}"|' /opt/nacos/bin/startup.sh \
    && sed -i 's|nohup.*|$JAVA ${JAVA_OPT} nacos.nacos|' /opt/nacos/bin/startup.sh

WORKDIR /opt/nacos
EXPOSE 8848 5005
ENV JAVA_OPT "-agentlib:jdwp=transport=dt_socket,server=y,suspend=n,address=5005"
CMD ["/opt/nacos/bin/startup.sh", "-m", "standalone"]
