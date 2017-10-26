FROM vulhub/tomcat:8.5

MAINTAINER phithon <root@leavesongs.com>

RUN set -ex \
    && rm -rf /usr/local/tomcat/webapps/* \
    && chmod a+x /usr/local/tomcat/bin/*.sh

ADD https://github.com/vulhub/package/raw/master/struts2/s2-016/ROOT.war /usr/local/tomcat/webapps/ROOT.war
EXPOSE 8080