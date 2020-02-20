FROM tomcat:9.0.30-jdk8-openjdk

LABEL maintainer="phithon <root@leavesongs.com>"

RUN set -ex \
    && sed -i 's/securerandom\.source=file:\/dev\/random/securerandom.source=file:\/dev\/.\/urandom/g' $JAVA_HOME/jre/lib/security/java.security \
    && rmdir webapps \
    && mv webapps.dist webapps