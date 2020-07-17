FROM maven:3-jdk-8 AS builder

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && wget -qO- https://github.com/apereo/cas-overlay-template/archive/4.1.tar.gz | tar xz --strip-components 1 -C /usr/src \
    && cd /usr/src \
    && sed -E -i 's#<cas\.version>4\.1\.[0-9]+</cas\.version>#<cas.version>4.1.5</cas.version>#' pom.xml \
    && mvn clean package

FROM tomcat:8.5-jdk8

COPY --from=builder /usr/src/target/cas.war /usr/local/tomcat/webapps/cas.war
COPY --from=builder /usr/src/etc/ /etc/cas/

RUN set -ex \
    && sed -i 's/securerandom\.source=file:\/dev\/random/securerandom.source=file:\/dev\/.\/urandom/g' $JAVA_HOME/jre/lib/security/java.security
