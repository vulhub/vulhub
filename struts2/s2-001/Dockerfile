FROM tomcat:8

RUN set -ex \
    && rm -rf /usr/local/tomcat/webapps/* \
    && chmod a+x /usr/local/tomcat/bin/*.sh

COPY S2-001.war /usr/local/tomcat/webapps/ROOT.war
EXPOSE 8080