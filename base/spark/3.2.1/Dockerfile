FROM eclipse-temurin:8-jre-jammy

LABEL maintainer="phithon <root@leavesongs.com>"

ARG SPARK_HOME=/usr/local/spark
ARG SPARK_DOWNLOAD_URL=https://archive.apache.org/dist/spark/spark-3.2.1/spark-3.2.1-bin-hadoop3.2.tgz

ENV SPARK_HOME=${SPARK_HOME} \
    PATH=${SPARK_HOME}/bin:${SPARK_HOME}/sbin:${PATH} \
    SPARK_NO_DAEMONIZE=1 \
    SPARK_DAEMON_JAVA_OPTS="-agentlib:jdwp=transport=dt_socket,server=y,suspend=n,address=5005"

SHELL ["/bin/bash", "-o", "pipefail", "-c"]

RUN set -ex \
    && apt-get update \
    && apt-get install --no-install-recommends -y ca-certificates procps wget \
    && mkdir -p "${SPARK_HOME}" \
    && wget -qO- "${SPARK_DOWNLOAD_URL}" | tar zx -C "${SPARK_HOME}" --strip-components=1 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR ${SPARK_HOME}

EXPOSE 8080 7077 5005

COPY docker-entrypoint.sh /docker-entrypoint.sh

ENTRYPOINT ["/docker-entrypoint.sh"]

CMD ["master"]
