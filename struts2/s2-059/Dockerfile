FROM vulhub/tomcat:8.5

MAINTAINER xinhaiyucheng <xinhaiyucheng@gmail.com>

RUN set -ex \
    && rm -rf /usr/local/tomcat/webapps/* \
    && chmod a+x /usr/local/tomcat/bin/*.sh

COPY s2-059_war.war /usr/local/tomcat/webapps/ROOT.war
EXPOSE 8080