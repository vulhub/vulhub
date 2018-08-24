FROM tomcat:8.5

LABEL maintainer="phithon <root@leavesongs.com>"

ARG ST2_VERSION=2.5.12
ARG WEB_DIR=/usr/local/tomcat/webapps

RUN set -ex \
    && rm -rf ${WEB_DIR}/* \
    && chmod a+x /usr/local/tomcat/bin/*.sh \
    \
    && wget http://archive.apache.org/dist/struts/${ST2_VERSION}/struts-${ST2_VERSION}-apps.zip -O /tmp/struts-${ST2_VERSION}-apps.zip \
    && unzip /tmp/struts-${ST2_VERSION}-apps.zip -d /tmp/ \
    && mv /tmp/struts-${ST2_VERSION}/apps/struts2-rest-showcase.war ${WEB_DIR}/ROOT.war  \
    && rm -rf /tmp/struts* \
    && sed -i 's/securerandom\.source=file:\/dev\/random/securerandom.source=file:\/dev\/.\/urandom/g' $JAVA_HOME/lib/security/java.security

EXPOSE 8080