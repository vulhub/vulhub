FROM openjdk:8-jdk

ARG JETTY_BASE=/opt/jetty
RUN set -ex \
    && wget -O/tmp/jetty.zip https://repo1.maven.org/maven2/org/eclipse/jetty/jetty-distribution/9.4.37.v20210219/jetty-distribution-9.4.37.v20210219.zip \
    && unzip -d /opt /tmp/jetty.zip \
    && mv /opt/jetty-* ${JETTY_BASE} \
    && rm -rf /tmp/jetty*

WORKDIR ${JETTY_BASE}
CMD ["/opt/jetty/bin/jetty.sh", "run"]
