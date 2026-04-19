FROM tomcat:9.0.116-jdk8

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && mv /usr/local/tomcat/webapps.dist/* /usr/local/tomcat/webapps/ \
    && curl -fSL https://repo1.maven.org/maven2/commons-collections/commons-collections/3.2.1/commons-collections-3.2.1.jar \
       -o /usr/local/tomcat/lib/commons-collections-3.2.1.jar

COPY server.xml /usr/local/tomcat/conf/server.xml
