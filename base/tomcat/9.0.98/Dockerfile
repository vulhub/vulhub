FROM tomcat:9.0.98-jre8-temurin-noble
COPY server.xml /usr/local/tomcat/conf/server.xml
COPY web.xml /usr/local/tomcat/conf/web.xml
COPY context.xml /usr/local/tomcat/conf/context.xml
COPY commons-collections-3.1.jar /usr/local/tomcat/lib/commons-collections-3.1.jar

RUN set -ex \
    && rmdir webapps \
    && mv webapps.dist webapps