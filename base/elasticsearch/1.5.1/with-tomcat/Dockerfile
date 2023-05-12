FROM vulhub/elasticsearch:1.5.1

LABEL maintainer="phithon <root@leavesongs.com>"

ENV CATALINA_HOME=/usr/local/tomcat

COPY docker-entrypoint.sh /docker-entrypoint.sh

RUN set -ex \
    && mkdir -p $CATALINA_HOME \
    && wget -qO- http://www-us.apache.org/dist/tomcat/tomcat-8/v8.5.33/bin/apache-tomcat-8.5.33.tar.gz \
        | tar zx --strip-components 1 -C $CATALINA_HOME \
    && sed -i 's/securerandom\.source=file:\/dev\/random/securerandom.source=file:\/dev\/.\/urandom/g' $JAVA_HOME/lib/security/java.security \
    && chmod +x /docker-entrypoint.sh
