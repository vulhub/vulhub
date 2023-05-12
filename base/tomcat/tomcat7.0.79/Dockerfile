FROM tomcat:7.0.79

LABEL maintainer="phithon <root@leavesongs.com>"

RUN sed -i 's/securerandom.source=file:\/dev\/urandom/securerandom.source=file:\/dev\/.\/urandom/g' $JAVA_HOME/lib/security/java.security