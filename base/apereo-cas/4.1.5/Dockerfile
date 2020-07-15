FROM maven:3-jdk-8 AS builder

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && wget -qO- https://github.com/apereo/cas-overlay-template/archive/4.1.tar.gz | tar xz --strip-components 1 -C /usr/src \
    && cd /usr/src \
    && mvn clean package

FROM tomcat:8.5-jdk8

COPY --from=builder /usr/src/target/cas.war /usr/local/tomcat/webapps/cas.war
