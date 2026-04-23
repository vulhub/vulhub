FROM eclipse-temurin:8u362-b09-jdk-jammy

LABEL maintainer="phithon <root@leavesongs.com>"

ENV LINKIS_HOME=/opt/linkis
ENV LINKIS_CONF_DIR=/opt/linkis/conf
ENV DEBIAN_FRONTEND=noninteractive

ARG LINKIS_URL=https://archive.apache.org/dist/linkis/release-1.3.0/apache-linkis-1.3.0-incubating-bin.tar.gz

RUN set -ex \
    && apt-get update \
    && apt-get install -y --no-install-recommends \
        sudo \
        default-mysql-client \
        lsof \
        net-tools \
        iproute2 \
        gawk \
        python3 \
        ca-certificates \
        curl \
        telnet \
        procps \
    && ln -sf bash /bin/sh \
    && ln -s /usr/bin/python3 /usr/bin/python \
    && useradd -m -s /bin/bash hadoop \
    && echo "hadoop ALL=(ALL) NOPASSWD: ALL" > /etc/sudoers.d/hadoop \
    && mkdir -p /opt/linkis-bin /tmp/linkis \
    && curl -fsSL -o /tmp/linkis.tar.gz "$LINKIS_URL" \
    && tar xzf /tmp/linkis.tar.gz -C /opt/linkis-bin \
    && rm -f /tmp/linkis.tar.gz \
    && curl -fsSL -o /opt/linkis-bin/linkis-package/lib/linkis-commons/public-module/mysql-connector-java-5.1.40.jar \
        "https://repo1.maven.org/maven2/mysql/mysql-connector-java/5.1.40/mysql-connector-java-5.1.40.jar" \
    && cp /opt/linkis-bin/linkis-package/lib/linkis-commons/public-module/mysql-connector-java-5.1.40.jar \
        /opt/linkis-bin/linkis-package/lib/linkis-spring-cloud-services/linkis-mg-gateway/ \
    && sed -i 's|need_cmd yum||' /opt/linkis-bin/bin/checkEnv.sh \
    && touch /home/hadoop/.bash_profile \
    && chown -R hadoop:hadoop /opt/linkis-bin /tmp/linkis /home/hadoop \
    && rm -rf /var/lib/apt/lists/*

COPY --chown=hadoop:hadoop linkis-env.sh /opt/linkis-bin/deploy-config/linkis-env.sh
COPY --chown=hadoop:hadoop db.sh /opt/linkis-bin/deploy-config/db.sh
COPY --chmod=0755 entrypoint.sh /entrypoint.sh

USER hadoop
WORKDIR /opt/linkis-bin

EXPOSE 9001 20303 9101 9105 5005

ENTRYPOINT ["/entrypoint.sh"]
