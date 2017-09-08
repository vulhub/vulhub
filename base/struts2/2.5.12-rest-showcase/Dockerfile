FROM vulhub/tomcat:8.5

MAINTAINER phithon <root@leavesongs.com>

RUN set -ex \
    && rm -rf /usr/local/tomcat/webapps/* \
    && chmod a+x /usr/local/tomcat/bin/*.sh \
    && apt-get update \
    && apt-get install -y unzip \
    && wget http://archive.apache.org/dist/struts/2.5.12/struts-2.5.12-all.zip -O /tmp/struts.zip \
    && unzip /tmp/struts.zip -d /tmp/ \
    && mv /tmp/struts-2.5.12/apps/struts2-rest-showcase.war /usr/local/tomcat/webapps/ROOT.war \
    && apt-get purge -y --auto-remove unzip \
    && rm -rf /var/lib/apt/lists/* \
    && rm -rf /tmp/struts*

EXPOSE 8080