FROM maven:3-jdk-11 AS builder

COPY ./ /usr/src

RUN cd /usr/src; \
    mvn -U clean package -Dmaven.test.skip=true -Dmaven.artifact.threads=10

FROM tomcat:8.5.77-jre11

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && rm -rf /usr/local/tomcat/webapps/* \
    && sed -i 's/securerandom\.source=file:\/dev\/random/securerandom.source=file:\/dev\/.\/urandom/g' $JAVA_HOME/conf/security/java.security

COPY --from=builder /usr/src/target/spring4shell.war /usr/local/tomcat/webapps/ROOT.war
EXPOSE 8080
