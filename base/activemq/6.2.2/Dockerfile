FROM eclipse-temurin:17-jre

LABEL maintainer="phithon <root@leavesongs.com>"

ENV ACTIVEMQ_VERSION=6.2.2
ENV ACTIVEMQ=apache-activemq-${ACTIVEMQ_VERSION}
ENV ACTIVEMQ_HOME=/opt/activemq

RUN set -ex \
    && mkdir -p "${ACTIVEMQ_HOME}" \
    && curl -fsSL "https://archive.apache.org/dist/activemq/${ACTIVEMQ_VERSION}/${ACTIVEMQ}-bin.tar.gz" \
        | tar xz --strip-components=1 -C "${ACTIVEMQ_HOME}" \
    && sed -i 's/127\.0\.0\.1/0.0.0.0/' "${ACTIVEMQ_HOME}/conf/jetty.xml" \
    && sed -i '/<strict-checking\/>/d' "${ACTIVEMQ_HOME}/conf/jolokia-access.xml"

WORKDIR ${ACTIVEMQ_HOME}

EXPOSE 61616 8161

CMD ["/opt/activemq/bin/activemq", "console"]
