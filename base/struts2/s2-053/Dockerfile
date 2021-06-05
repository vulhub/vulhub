FROM vulhub/tomcat:8.5

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && rm -rf /usr/local/tomcat/webapps/* \
    && chmod a+x /usr/local/tomcat/bin/*.sh

ADD https://cdn.vulhub.org/struts2/s2-053/ROOT.war /usr/local/tomcat/webapps/ROOT.war
EXPOSE 8080