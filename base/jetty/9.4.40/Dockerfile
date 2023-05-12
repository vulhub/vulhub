FROM openjdk:8-jdk

ARG JETTY_BASE=/opt/jetty
RUN set -ex \
    && wget -O/tmp/jetty.zip https://repo1.maven.org/maven2/org/eclipse/jetty/jetty-distribution/9.4.40.v20210413/jetty-distribution-9.4.40.v20210413.zip \
    && unzip -d /opt /tmp/jetty.zip \
    && mv /opt/jetty-* ${JETTY_BASE} \
    && echo '--module=servlets' >> ${JETTY_BASE}/start.ini \
    && rm -rf /tmp/jetty*

WORKDIR ${JETTY_BASE}
CMD ["/opt/jetty/bin/jetty.sh", "run"]
